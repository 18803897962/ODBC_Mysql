#pragma once
#include<iostream>
#include<mysql.h>
class CMysql
{
public:
	enum {
		MYSQL_USE_STREAM = 1,
		MYSQL_STORE_STREAM
	};

	static CMysql* getInstance() {
		if (m_instance == NULL) {
			m_instance = new CMysql();
		}
		return m_instance;
	}

	operator MYSQL* () {
		return this->mysql;
	}

	int Login(bool isRecover = false) {
		pDB = mysql_real_connect(mysql, pUserInfo->host.c_str(), pUserInfo->user.c_str(), pUserInfo->passwd.c_str(), 
			pUserInfo->db.c_str(), pUserInfo->port, NULL, !isRecover ? 0 : CLIENT_LOCAL_FILES);
		if (pDB == NULL) {
			ShowError("Login failed");
			return -1;
		}
		return 0;
	}

	int SetUserInfo(const std::string& host, const std::string& user, const std::string& passwd, const std::string& db, unsigned int port) {
		pUserInfo->host = host;
		pUserInfo->user = user;
		pUserInfo->passwd = passwd;
		pUserInfo->db = db;
		pUserInfo->port = port;
	}

	int Set_EncodingFormat(const std::string& Format) {
		std::string sql = "SET NAMES '"+Format+"';";
		int ret = this->Query(sql);
		if (ret != 0) {
			ShowError("set encoding format failed");
			return -1;
		}
		sql = "SET CHARACTER SET " + Format + ";";
		ret = this->Query(sql);
		if (ret != 0) {
			ShowError("set encoding format failed");
			return -2;
		}
		setlocale(LOCALE_ALL, "en_GB.UTF-8");//设置控制台编码
		return 0;
	}
	//注：若设置了不自动提交模式 需要自行提交才能生效
	//请在使用完之后将其设回自动提交模式，避免影响其它数据库操作
	int Set_AutoCommit(bool Automode = true) {
		int ret = mysql_autocommit(mysql, Automode);
		if (ret != 0) {
			ShowError("set auto commit mode failed");
			return -1;
		}
		return 0;
	}

	int Commit() {
		int ret = mysql_commit(mysql);
		if (ret != 0) {
			ShowError("set auto commit mode failed");
			return -1;
		}
		return 0;
	}

	int Rollback() {
		int ret = mysql_rollback(mysql);
		if (ret != 0) {
			ShowError("roll back failed");
			return -1;
		}
		return 0;
	}

	int Query(const std::string& cmd) {
		int ret = mysql_real_query(mysql,cmd.c_str(),(unsigned long)cmd.size());
		if (ret != 0) {
			ShowError("Query failed");
			return -1;
		}
		return 0;
	}

	//关于MYSQL_SHOW_STREAM的选项和说明如下
	//MYSQL_STORE_STREAM 本次查询所有结果均存储到客户端 支持顺序访问、随机访问 效率相对较低
	//MYSQL_USE_STREAM 本次查询只在客户端缓存一行结果 支持顺序访问 效率较高
	int ShowResult(int MYSQL_SHOW_STREAM = MYSQL_STORE_STREAM) {
		//mysql_store_result
		if (MYSQL_SHOW_STREAM == MYSQL_STORE_STREAM) {
			result = mysql_store_result(mysql);
			if (result == NULL) {
				ShowError("show result failed");
				return -1;
			}
			unsigned int nFields = mysql_num_fields(result);
			my_ulonglong nRows = mysql_num_rows(result);
			MYSQL_FIELD* fields = mysql_fetch_fields(result);
			for (unsigned i = 0; i < nFields; i++) {
				std::cout << fields[i].name << " ";
			}
			std::cout << std::endl;
			for (unsigned i = 0; i < nRows; i++) {
				MYSQL_ROW row = mysql_fetch_row(result);
				for (unsigned j = 0; j < nFields; j++) {
					std::cout << row[j] << " ";
				}
				std::cout << std::endl;
			}
		}
		else if (MYSQL_SHOW_STREAM == MYSQL_USE_STREAM) {
			result = mysql_use_result(mysql);
			if (result == NULL) {
				ShowError("show result failed");
				return -2;
			}
			unsigned int nFields = mysql_num_fields(result);
			my_ulonglong nRows = mysql_num_rows(result);
			MYSQL_FIELD* fields = mysql_fetch_fields(result);
			MYSQL_ROW row;
			for (unsigned i = 0; i < nFields; i++) {
				std::cout << fields[i].name << " ";
			}
			do {
				row = mysql_fetch_row(result);//此时才真正开始查询
				if (row != NULL) {
					for (unsigned j = 0; j < nFields; j++) {
						std::cout << row[j] << " ";
					}
					std::cout << std::endl;
				}
			} while (row != NULL);
		}
		std::cout << "------------------------------------------------------------------" << std::endl;
		mysql_free_result(result);
	}

	void ShowError(const std::string& str = "") {
		std::cout << "mysql error!" << str << std::endl;
		std::cout << "error code:" << mysql_errno(mysql)<< std::endl;
		std::cout << "error info:" << mysql_error(mysql)<<std::endl;
	}
private:
	CMysql() {
		mysql = new MYSQL();
		pUserInfo = new UserINFO("localhost","root","root","mysql",3306);
		if (mysql == NULL) {
			std::cout << "construct failed" << std::endl;
		}
		pDB = mysql_init(mysql);
		if (pDB == NULL) {
			ShowError("init failed");
		}
		result = NULL;
	}

	~CMysql() {
		if (mysql != NULL) {
			MYSQL* tmp = mysql;
			mysql = pDB = NULL;
			delete tmp;
		}
		if (pUserInfo != NULL) {
			UserINFO* tmp = pUserInfo;
			pUserInfo = NULL;
			delete pUserInfo;
		}
		mysql_close(mysql);
	}

	CMysql(const CMysql& cmysql) {
		mysql = cmysql.mysql;
		pDB = cmysql.pDB;
		result = cmysql.result;
		pUserInfo = new UserINFO(*cmysql.pUserInfo);
	}

	CMysql& operator=(const CMysql& cmysql) {
		if (this != &cmysql) {
			mysql = cmysql.mysql;
			pDB = cmysql.pDB;
			result = cmysql.result;
			pUserInfo = new UserINFO(*cmysql.pUserInfo);
		}
		return *this;
	}

	static void releaseInstance() {
		if (m_instance != NULL) {
			CMysql* tmp = m_instance;
			m_instance = NULL;
			delete tmp;
		}
	}

	class CHelper {
	public:
		CHelper() {
			CMysql::getInstance();
		}
		~CHelper() {
			CMysql::releaseInstance();
		}
	};
	
	struct UserINFO
	{
		/*
		MYSQL *STDCALL mysql_real_connect(MYSQL *mysql, const char *host,
                                  const char *user, const char *passwd,
                                  const char *db, unsigned int port,
                                  const char *unix_socket,
                                  unsigned long clientflag);
		*/
		std::string host, user, passwd, db;
		unsigned int port;
		UserINFO() : port(-1) {}
		UserINFO( const std::string& host,
			const std::string& user, const std::string& passwd,
			const std::string& db, unsigned int port):
			host(host), user(user), passwd(passwd), db(db), port(port) {}
		UserINFO(const UserINFO& info) {
			host = info.host;
			user = info.user;
			passwd = info.passwd;
			db = info.db;
			port = info.port;
		}
		UserINFO& operator=(const UserINFO& info) {
			if (this != &info) {
				host = info.host;
				user = info.user;
				passwd = info.passwd;
				db = info.db;
				port = info.port;
			}
			return *this;
		}
	};
private:
	MYSQL* mysql;
	MYSQL* pDB;
	MYSQL_RES* result;
	static CMysql* m_instance;
	static CHelper m_helper;
	UserINFO* pUserInfo;
};

