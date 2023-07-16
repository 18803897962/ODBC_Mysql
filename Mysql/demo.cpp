#include"CMysql.h"
void show_result(MYSQL_RES* result) {
	unsigned nFileds = mysql_num_fields(result);
	my_ulonglong nRows = mysql_num_rows(result);
	MYSQL_FIELD* fields = mysql_fetch_fields(result);
	MYSQL_ROW row;
	do {
		row = mysql_fetch_row(result);//此时才真正开始查询
		if (row != NULL) {
			for (unsigned j = 0; j < nFileds; j++) {
				std::cout << "type:" << fields[j].type << " " << fields[j].name << ":" << row[j] << std::endl;
			}
		}
	} while (row != NULL);
}

int Login(MYSQL*& mysql, MYSQL*& tmp, bool isRecover = false) {
	mysql = new MYSQL();
	tmp = mysql_init(mysql);
	if (tmp == NULL) {
		std::cout << "mysql error: " << mysql_error(mysql) << std::endl;
		return -1;
	}
	if (!isRecover) {
		tmp = mysql_real_connect(tmp, "localhost", "root", "wang410726", "mysql", 3306, NULL, 0);
	}
	else {
		tmp = mysql_real_connect(tmp, "localhost", "root", "wang410726", "mysql", 3306, NULL, CLIENT_LOCAL_FILES);
	}
	if (tmp == NULL) {
		std::cout << "mysql error: " << mysql_error(mysql) << std::endl;
		return -2;
	}
	return 0;
}

int lession4() {//登录数据库
	MYSQL* mysql = new MYSQL();
	MYSQL* pDB = mysql_init(mysql);
	if (pDB == NULL) {
		std::cout << "mysql_init failed" << std::endl;
		return 0;
	}
	pDB = mysql_real_connect(pDB, "localhost", "root", "wang410726", "mysql", 3306, NULL, 0);
	//host 可以填 [ip、localhost、域名]  localhost就是本地主机，即127.0.0.1

	std::cout << pDB << std::endl;
	mysql_close(pDB);
	delete mysql;
	return 0;
}

int lession7() {//创建用户+授权
	MYSQL* mysql = new MYSQL();
	MYSQL* pDB = mysql_init(mysql);
	if (pDB == NULL) {
		std::cout << "mysql_init failed" << std::endl;
		return 0;
	}
	pDB = mysql_real_connect(pDB, "localhost", "root", "wang410726", "mysql", 3306, NULL, 0);
	//host 可以填 [ip、localhost、域名]  localhost就是本地主机，即127.0.0.1
	if (pDB) {
		std::string sql = "CREATE USER 'hello'@'localhost' IDENTIFIED BY '123456'";
		int ret = mysql_real_query(pDB, sql.c_str(), (unsigned long)sql.size());
		if (ret != 0) {
			std::cout << "mysql error:" << mysql_error(pDB) << std::endl;
		}
		MYSQL_RES* result = mysql_use_result(mysql);
		if (result != NULL) {
			show_result(result);
			mysql_free_result(result);
		}

		sql = "GRANT ALL ON *.* TO 'hello'@'localhost'";
		ret = mysql_real_query(pDB, sql.c_str(), (unsigned long)sql.size());
		if (ret != 0) {
			std::cout << "mysql error:" << mysql_error(pDB) << std::endl;
		}
		result = mysql_use_result(pDB);
		if (result != NULL) {
			show_result(result);
			mysql_free_result(result);
		}
	}

	mysql_close(pDB);
	delete mysql;
	return 0;
}

int lession8() {//创建仓库
	MYSQL* mysql = NULL;
	MYSQL* pDB = NULL;
	MYSQL_RES* result = NULL;
	int ret = Login(mysql, pDB);
	if (ret != 0) {
		std::cout << "login failed" << std::endl;
		return -1;
	}
	if (pDB) {
		std::string sql = "CREATE DATABASE hello";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(mysql) << std::endl;
			return -2;
		}
		result = mysql_use_result(mysql);
		if (ret != NULL) {
			show_result(result);
			mysql_free_result(result);
		}
		sql = "USE hello;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(mysql) << std::endl;
			return -2;
		}
		result = mysql_use_result(mysql);
		if (ret != NULL) {
			show_result(result);
			mysql_free_result(result);
		}
		sql = "DROP DATABASE hello;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(mysql) << std::endl;
			return -2;
		}
		result = mysql_use_result(mysql);
		if (ret != NULL) {
			show_result(result);
			mysql_free_result(result);
		}
		mysql_close(mysql);
	}
	delete mysql;
	return 0;
}

int lession9() {
	MYSQL* mysql = NULL;
	MYSQL* pDB = NULL;
	MYSQL_RES* result = NULL;
	int ret = Login(mysql, pDB);
	if (ret != 0) {
		std::cout << "login failed" << std::endl;
		return -1;
	}
	if (pDB) {
		std::string sql = "CREATE DATABASE hello";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		result = mysql_use_result(mysql);
		if (ret != NULL) {
			show_result(result);
			mysql_free_result(result);
		}
		sql = "USE hello;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		sql = "CREATE TABLE IF NOT EXISTS `hello` (`编号` NVARCHAR(16) PRIMARY KEY)ENGINE=InnoDB DEFAULT CHARSET=utf8;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			sql = "DROP DATABASE hello; ";
			ret = mysql_real_query(pDB, sql.c_str(), sql.size());
			if (ret != 0) {
				std::cout << "mysql error: " << mysql_error(mysql) << std::endl;
				return -2;
			}
			mysql_close(mysql);
			return -2;
		}
		sql = "DROP TABLE `hello`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		sql = "DROP DATABASE hello; ";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(mysql) << std::endl;
			return -2;
		}
		mysql_close(mysql);
	}
	delete mysql;
	return 0;
}

int lession10() {
	MYSQL* mysql = NULL;
	MYSQL* pDB = NULL;
	MYSQL_RES* result = NULL;
	int ret = Login(mysql, pDB);
	if (ret != 0) {
		std::cout << "login failed" << std::endl;
		return -1;
	}
	if (pDB) {
		std::string sql = "CREATE DATABASE hello";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		result = mysql_use_result(mysql);
		if (ret != NULL) {
			show_result(result);
			mysql_free_result(result);
		}
		sql = "USE hello;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		sql = "CREATE TABLE IF NOT EXISTS `hello` (`编号` NVARCHAR(16) PRIMARY KEY)ENGINE=InnoDB DEFAULT CHARSET=utf8;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			sql = "DROP DATABASE hello; ";
			ret = mysql_real_query(pDB, sql.c_str(), sql.size());
			if (ret != 0) {
				std::cout << "mysql error: " << mysql_error(mysql) << std::endl;
				return -2;
			}
			mysql_close(mysql);
			return -2;
		}

		sql = "INSERT INTO `hello` (`编号`) VALUES (\"9527\");";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		sql = "DROP TABLE `hello`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		sql = "DROP DATABASE hello; ";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(mysql) << std::endl;
			return -2;
		}
		mysql_close(mysql);
	}
	delete mysql;
	return 0;
}

int lession11() {
	MYSQL* mysql = NULL;
	MYSQL* pDB = NULL;
	MYSQL_RES* result = NULL;
	int ret = Login(mysql, pDB);
	if (ret != 0) {
		std::cout << "login failed" << std::endl;
		return -1;
	}
	if (pDB) {
		std::string sql = "CREATE DATABASE hello";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		result = mysql_use_result(mysql);
		if (ret != NULL) {
			show_result(result);
			mysql_free_result(result);
		}
		sql = "USE hello;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		sql = "CREATE TABLE IF NOT EXISTS `hello` (`编号` NVARCHAR(16) PRIMARY KEY)ENGINE=InnoDB DEFAULT CHARSET=utf8;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			sql = "DROP DATABASE hello; ";
			ret = mysql_real_query(pDB, sql.c_str(), sql.size());
			if (ret != 0) {
				std::cout << "mysql error: " << mysql_error(mysql) << std::endl;
				return -2;
			}
			mysql_close(mysql);
			return -2;
		}

		sql = "INSERT INTO `hello` (`编号`) VALUES (\"9527\");";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "INSERT INTO `hello` (`编号`) VALUES (\"9528\");";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "DELETE FROM `hello` WHERE `编号`=\"9527\";";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "DROP TABLE `hello`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		sql = "DROP DATABASE hello; ";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(mysql) << std::endl;
			return -2;
		}
		mysql_close(mysql);
	}
	delete mysql;
	return 0;
}

int lession12() {
	MYSQL* mysql = NULL;
	MYSQL* pDB = NULL;
	MYSQL_RES* result = NULL;
	int ret = Login(mysql, pDB);
	if (ret != 0) {
		std::cout << "login failed" << std::endl;
		return -1;
	}
	if (pDB) {
		std::string sql = "CREATE DATABASE hello";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		result = mysql_use_result(mysql);
		if (ret != NULL) {
			show_result(result);
			mysql_free_result(result);
		}
		sql = "USE hello;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		sql = "CREATE TABLE IF NOT EXISTS `hello` (`编号` NVARCHAR(16) PRIMARY KEY";
		sql += ",`age` INT NOT NULL DEFAULT 18";
		sql += ")ENGINE = InnoDB DEFAULT CHARSET = utf8; ";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			sql = "DROP DATABASE hello; ";
			ret = mysql_real_query(pDB, sql.c_str(), sql.size());
			if (ret != 0) {
				std::cout << "mysql error: " << mysql_error(mysql) << std::endl;
				return -2;
			}
			mysql_close(mysql);
			return -2;
		}

		sql = "INSERT INTO `hello` (`编号`,`age`) VALUES (\"9527\",35);";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "INSERT INTO `hello` (`编号`,`age`) VALUES (\"9528\",99);";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "UPDATE `hello` SET age=44 WHERE `编号`=\"9528\";";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "UPDATE `hello` SET age=18 WHERE `age`>35;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "DELETE FROM `hello` WHERE `编号`=\"9527\";";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "DROP TABLE `hello`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		sql = "DROP DATABASE hello; ";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(mysql) << std::endl;
			return -2;
		}
		mysql_close(mysql);
	}
	delete mysql;
	return 0;
}

int lession13() {
	MYSQL* mysql = NULL;
	MYSQL* pDB = NULL;
	MYSQL_RES* result = NULL;
	int ret = Login(mysql, pDB);
	if (ret != 0) {
		std::cout << "login failed" << std::endl;
		return -1;
	}
	if (pDB) {
		std::string sql = "CREATE DATABASE hello";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		result = mysql_use_result(mysql);
		if (ret != NULL) {
			show_result(result);
			mysql_free_result(result);
		}
		sql = "USE hello;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		sql = "CREATE TABLE IF NOT EXISTS `hello` (`编号` NVARCHAR(16) PRIMARY KEY";
		sql += ",`age` INT NOT NULL DEFAULT 18";
		sql += ")ENGINE = InnoDB DEFAULT CHARSET = utf8; ";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			sql = "DROP DATABASE hello; ";
			ret = mysql_real_query(pDB, sql.c_str(), sql.size());
			if (ret != 0) {
				std::cout << "mysql error: " << mysql_error(mysql) << std::endl;
				return -2;
			}
			mysql_close(mysql);
			return -2;
		}

		sql = "INSERT INTO `hello` (`编号`,`age`) VALUES (\"9527\",35);";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		result = mysql_use_result(pDB);
		if (result != NULL) {
			show_result(result);
			std::cout << "=================================================" << std::endl;
			mysql_free_result(result);
		}

		sql = "INSERT INTO `hello` (`编号`,`age`) VALUES (\"9528\",99);";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "SELECT * FROM `hello`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		result = mysql_use_result(pDB);
		if (result != NULL) {
			show_result(result);
			std::cout << "=================================================" << std::endl;
			mysql_free_result(result);
		}

		sql = "UPDATE `hello` SET age=44 WHERE `编号`=\"9528\";";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "SELECT * FROM `hello`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		result = mysql_use_result(pDB);
		if (result != NULL) {
			show_result(result);
			std::cout << "=================================================" << std::endl;
			mysql_free_result(result);
		}

		sql = "UPDATE `hello` SET age=18 WHERE `age`>35;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		sql = "SELECT * FROM `hello`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		result = mysql_use_result(pDB);
		if (result != NULL) {
			show_result(result);
			std::cout << "=================================================" << std::endl;
			mysql_free_result(result);
		}

		sql = "DELETE FROM `hello` WHERE `编号`=\"9527\";";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "SELECT * FROM `hello`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		result = mysql_use_result(pDB);
		if (result != NULL) {
			show_result(result);
			std::cout << "=================================================" << std::endl;
			mysql_free_result(result);
		}

		sql = "DROP TABLE `hello`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		sql = "DROP DATABASE hello; ";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(mysql) << std::endl;
			return -2;
		}
		mysql_close(mysql);
	}
	delete mysql;
	return 0;
}

int lession14() {
	MYSQL* mysql = NULL;
	MYSQL* pDB = NULL;
	MYSQL_RES* result = NULL;
	int ret = Login(mysql, pDB);
	if (ret != 0) {
		std::cout << "login failed" << std::endl;
		return -1;
	}
	if (pDB) {
		std::string sql = "CREATE DATABASE hello";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "SET NAMES 'utf8';";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		sql = "SET CHARACTER SET utf8;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		/*
		SET NAMES 'utf8'
		SET CHARACTER SET utf8
		*/
		sql = "USE hello;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		sql = "CREATE TABLE IF NOT EXISTS `hello` (`编号` NVARCHAR(16) PRIMARY KEY";
		sql += ",`age` INT NOT NULL DEFAULT 18";
		sql += ")ENGINE = InnoDB DEFAULT CHARSET = utf8; ";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			sql = "DROP DATABASE hello; ";
			ret = mysql_real_query(pDB, sql.c_str(), sql.size());
			if (ret != 0) {
				std::cout << "mysql error: " << mysql_error(mysql) << std::endl;
				return -2;
			}
			mysql_close(mysql);
			return -2;
		}

		sql = "CREATE TABLE IF NOT EXISTS `teacher` (`编号` NVARCHAR(16) PRIMARY KEY";
		sql += ",`age` INT NOT NULL DEFAULT 18";
		sql += ")ENGINE = InnoDB DEFAULT CHARSET = utf8; ";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			sql = "DROP DATABASE hello; ";
			ret = mysql_real_query(pDB, sql.c_str(), sql.size());
			if (ret != 0) {
				std::cout << "mysql error: " << mysql_error(mysql) << std::endl;
				return -2;
			}
			mysql_close(mysql);
			return -2;
		}

		sql = "INSERT INTO `teacher` (`编号`,`age`) VALUES (\"9527\",35);";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "INSERT INTO `teacher` (`编号`,`age`) VALUES (\"9529\",66);";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "INSERT INTO `teacher` (`编号`,`age`) VALUES (\"9526\",16);";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "INSERT INTO `hello` (`编号`,`age`) VALUES (\"9527\",35);";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "INSERT INTO `hello` (`编号`,`age`) VALUES (\"9526\",33);";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		result = mysql_use_result(pDB);
		if (result != NULL) {
			show_result(result);
			std::cout << "=================================================" << std::endl;
			mysql_free_result(result);
		}

		sql = "INSERT INTO `hello` (`编号`,`age`) VALUES (\"9528\",99);";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "SELECT age FROM `hello`	UNION ALL SELECT age FROM`teacher`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		result = mysql_store_result(pDB);
		if (result != NULL) {
			unsigned nFileds = mysql_num_fields(result);
			my_ulonglong nRows = mysql_num_rows(result);
			MYSQL_FIELD* fields = mysql_fetch_fields(result);
			for (unsigned i = 0; i < nRows; i++) {
				MYSQL_ROW row = mysql_fetch_row(result);//此时才真正开始查询
				if (row != NULL) {
					for (unsigned j = 0; j < nFileds; j++) {
						std::cout << "type:" << fields[j].type << " " << fields[j].name << ":" << row[j] << std::endl;
					}
				}
			}
			std::cout << "=================================================" << std::endl;
			mysql_free_result(result);
		}

		sql = "SELECT * FROM `hello`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		result = mysql_use_result(pDB);
		if (result != NULL) {
			show_result(result);
			std::cout << "=================================================" << std::endl;
			mysql_free_result(result);
		}

		sql = "UPDATE `hello` SET age=44 WHERE `编号`=\"9528\";";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "SELECT * FROM `hello`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		result = mysql_use_result(pDB);
		if (result != NULL) {
			show_result(result);
			std::cout << "=================================================" << std::endl;
			mysql_free_result(result);
		}

		sql = "UPDATE `hello` SET age=18 WHERE `age`>35;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		sql = "SELECT * FROM `hello`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		result = mysql_use_result(pDB);
		if (result != NULL) {
			show_result(result);
			std::cout << "=================================================" << std::endl;
			mysql_free_result(result);
		}

		sql = "CREATE INDEX age_index ON hello (age);";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}



		sql = "DELETE FROM `hello` WHERE `编号`=\"9527\";";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "SELECT * FROM `hello`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		result = mysql_use_result(pDB);
		if (result != NULL) {
			show_result(result);
			std::cout << "=================================================" << std::endl;
			mysql_free_result(result);
		}

		sql = "DROP TABLE `hello`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		sql = "DROP TABLE `teacher`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		sql = "DROP DATABASE hello; ";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(mysql) << std::endl;
			return -2;
		}
		mysql_close(mysql);
	}
	delete mysql;
	return 0;
}

int lession16() {
	MYSQL* mysql = NULL;
	MYSQL* pDB = NULL;
	MYSQL_RES* result = NULL;
	int ret = Login(mysql, pDB);
	if (ret != 0) {
		std::cout << "login failed" << std::endl;
		return -1;
	}
	if (pDB) {
		std::string sql = "CREATE DATABASE hello";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "SET NAMES 'utf8';";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		sql = "SET CHARACTER SET utf8;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		/*
		SET NAMES 'utf8'
		SET CHARACTER SET utf8
		*/
		sql = "USE hello;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		sql = "CREATE TABLE IF NOT EXISTS `hello` (`编号` NVARCHAR(16) PRIMARY KEY";
		sql += ",`age` INT NOT NULL DEFAULT 18";
		sql += ")ENGINE = InnoDB DEFAULT CHARSET = utf8; ";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			sql = "DROP DATABASE hello; ";
			ret = mysql_real_query(pDB, sql.c_str(), sql.size());
			if (ret != 0) {
				std::cout << "mysql error: " << mysql_error(mysql) << std::endl;
				return -2;
			}
			mysql_close(mysql);
			return -2;
		}

		sql = "CREATE TABLE IF NOT EXISTS `teacher` (`编号` NVARCHAR(16) PRIMARY KEY";
		sql += ",`age` INT NOT NULL DEFAULT 18";
		sql += ")ENGINE = InnoDB DEFAULT CHARSET = utf8; ";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			sql = "DROP DATABASE hello; ";
			ret = mysql_real_query(pDB, sql.c_str(), sql.size());
			if (ret != 0) {
				std::cout << "mysql error: " << mysql_error(mysql) << std::endl;
				return -2;
			}
			mysql_close(mysql);
			return -2;
		}

		sql = "CREATE TABLE IF NOT EXISTS `logs` (`ID` INT(11) NOT NULL AUTO_INCREMENT";
		sql += ", `log` VARCHAR(256) DEFAULT NULL COMMENT \"日志说明\"";
		sql += ",PRIMARY KEY(`ID`)";
		sql += ")ENGINE = InnoDB DEFAULT CHARSET = utf8mb4 COMMENT = \"日志\";";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		//写多行时，需要加一个\n \  这个反斜杠后面什么都不能加！！！只能换行！
		sql = "CREATE TRIGGER hello_log AFTER INSERT ON `hello` FOR EACH ROW \n \
		 BEGIN\n \
		 DECLARE s1 VARCHAR(40)character set utf8;\n \
		 DECLARE s2 VARCHAR(20) character set utf8;\n \
		 SET s2 = \" is created\";\n \
		 SET s1 = CONCAT(NEW.`编号`, s2);\n \
		 INSERT INTO logs(log) values(s1);\n \
		 END;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		ret = mysql_autocommit(pDB, false);//设置事务  false后面需要和true成对出现，不能影响后续mysql的操作
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "INSERT INTO `teacher` (`编号`,`age`) VALUES (\"9527\",35);";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		ret = mysql_rollback(pDB);
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		sql = "INSERT INTO `teacher` (`编号`,`age`) VALUES (\"9529\",66);";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "INSERT INTO `teacher` (`编号`,`age`) VALUES (\"9526\",16);";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		ret = mysql_commit(pDB);
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		sql = "INSERT INTO `hello` (`编号`,`age`) VALUES (\"9527\",35);";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "INSERT INTO `hello` (`编号`,`age`) VALUES (\"9526\",33);";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		result = mysql_use_result(pDB);
		if (result != NULL) {
			show_result(result);
			std::cout << "=================================================" << std::endl;
			mysql_free_result(result);
		}

		sql = "INSERT INTO `hello` (`编号`,`age`) VALUES (\"9528\",99);";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		ret = mysql_autocommit(pDB, true);//设置事务
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}


		sql = "SELECT age FROM `hello`	UNION ALL SELECT age FROM`teacher`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		result = mysql_store_result(pDB);
		if (result != NULL) {
			unsigned nFileds = mysql_num_fields(result);
			my_ulonglong nRows = mysql_num_rows(result);
			MYSQL_FIELD* fields = mysql_fetch_fields(result);
			for (unsigned i = 0; i < nRows; i++) {
				MYSQL_ROW row = mysql_fetch_row(result);//此时才真正开始查询
				if (row != NULL) {
					for (unsigned j = 0; j < nFileds; j++) {
						std::cout << "type:" << fields[j].type << " " << fields[j].name << ":" << row[j] << std::endl;
					}
				}
			}
			std::cout << "=================================================" << std::endl;
			mysql_free_result(result);
		}

		sql = "SELECT * FROM `hello`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		result = mysql_use_result(pDB);
		if (result != NULL) {
			show_result(result);
			std::cout << "=================================================" << std::endl;
			mysql_free_result(result);
		}

		sql = "UPDATE `hello` SET age=44 WHERE `编号`=\"9528\";";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "SELECT * FROM `hello`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		result = mysql_use_result(pDB);
		if (result != NULL) {
			show_result(result);
			std::cout << "=================================================" << std::endl;
			mysql_free_result(result);
		}

		sql = "UPDATE `hello` SET age=18 WHERE `age`>35;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		sql = "SELECT * FROM `hello`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		result = mysql_use_result(pDB);
		if (result != NULL) {
			show_result(result);
			std::cout << "=================================================" << std::endl;
			mysql_free_result(result);
		}

		sql = "CREATE INDEX age_index ON hello (age);";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}



		sql = "DELETE FROM `hello` WHERE `编号`=\"9527\";";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "SELECT * FROM `hello`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		result = mysql_use_result(pDB);
		if (result != NULL) {
			show_result(result);
			std::cout << "=================================================" << std::endl;
			mysql_free_result(result);
		}

		sql = "DROP TABLE `hello`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		sql = "DROP TABLE `teacher`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		sql = "DROP DATABASE hello; ";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(mysql) << std::endl;
			return -2;
		}
		mysql_close(mysql);
	}
	delete mysql;
	return 0;
}

int lession18() {
	MYSQL* mysql = NULL;
	MYSQL* pDB = NULL;
	MYSQL_RES* result = NULL;
	int ret = Login(mysql, pDB, true);
	if (ret != 0) {
		std::cout << "login failed" << std::endl;
		return -1;
	}
	if (pDB) {
		std::string sql = "CREATE DATABASE hello";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "SET NAMES 'utf8';";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		sql = "SET CHARACTER SET utf8;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		/*
		SET NAMES 'utf8'
		SET CHARACTER SET utf8
		*/
		sql = "USE hello;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		sql = "CREATE TABLE IF NOT EXISTS `hello` (`编号` NVARCHAR(16) PRIMARY KEY";
		sql += ",`age` INT NOT NULL DEFAULT 18";
		sql += ")ENGINE = InnoDB DEFAULT CHARSET = utf8; ";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			sql = "DROP DATABASE hello; ";
			ret = mysql_real_query(pDB, sql.c_str(), sql.size());
			if (ret != 0) {
				std::cout << "mysql error: " << mysql_error(mysql) << std::endl;
				return -2;
			}
			mysql_close(mysql);
			return -2;
		}

		sql = "CREATE TABLE IF NOT EXISTS `teacher` (`编号` NVARCHAR(16) PRIMARY KEY";
		sql += ",`age` INT NOT NULL DEFAULT 18";
		sql += ")ENGINE = InnoDB DEFAULT CHARSET = utf8; ";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			sql = "DROP DATABASE hello; ";
			ret = mysql_real_query(pDB, sql.c_str(), sql.size());
			if (ret != 0) {
				std::cout << "mysql error: " << mysql_error(mysql) << std::endl;
				return -2;
			}
			mysql_close(mysql);
			return -2;
		}

		sql = "CREATE TABLE IF NOT EXISTS `logs` (`ID` INT(11) NOT NULL AUTO_INCREMENT";
		sql += ", `log` VARCHAR(256) DEFAULT NULL COMMENT \"日志说明\"";
		sql += ",PRIMARY KEY(`ID`)";
		sql += ")ENGINE = InnoDB DEFAULT CHARSET = utf8mb4 COMMENT = \"日志\";";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		//写多行时，需要加一个\n \  这个反斜杠后面什么都不能加！！！只能换行！
		sql = "CREATE TRIGGER hello_log AFTER INSERT ON `hello` FOR EACH ROW \n \
		 BEGIN\n \
		 DECLARE s1 VARCHAR(40)character set utf8;\n \
		 DECLARE s2 VARCHAR(20) character set utf8;\n \
		 SET s2 = \" is created\";\n \
		 SET s1 = CONCAT(NEW.`编号`, s2);\n \
		 INSERT INTO logs(log) values(s1);\n \
		 END;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		ret = mysql_autocommit(pDB, false);//设置事务  false后面需要和true成对出现，不能影响后续mysql的操作
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "INSERT INTO `teacher` (`编号`,`age`) VALUES (\"9527\",35);";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		ret = mysql_rollback(pDB);
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		sql = "INSERT INTO `teacher` (`编号`,`age`) VALUES (\"9529\",66);";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "INSERT INTO `teacher` (`编号`,`age`) VALUES (\"9526\",16);";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		ret = mysql_commit(pDB);
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		sql = "INSERT INTO `hello` (`编号`,`age`) VALUES (\"9527\",35);";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "INSERT INTO `hello` (`编号`,`age`) VALUES (\"9526\",33);";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		result = mysql_use_result(pDB);
		if (result != NULL) {
			show_result(result);
			std::cout << "=================================================" << std::endl;
			mysql_free_result(result);
		}

		sql = "INSERT INTO `hello` (`编号`,`age`) VALUES (\"9528\",99);";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		ret = mysql_autocommit(pDB, true);//设置事务
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}


		sql = "SELECT age FROM `hello`	UNION ALL SELECT age FROM`teacher`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		result = mysql_store_result(pDB);
		if (result != NULL) {
			unsigned nFileds = mysql_num_fields(result);
			my_ulonglong nRows = mysql_num_rows(result);
			MYSQL_FIELD* fields = mysql_fetch_fields(result);
			for (unsigned i = 0; i < nRows; i++) {
				MYSQL_ROW row = mysql_fetch_row(result);//此时才真正开始查询
				if (row != NULL) {
					for (unsigned j = 0; j < nFileds; j++) {
						std::cout << "type:" << fields[j].type << " " << fields[j].name << ":" << row[j] << std::endl;
					}
				}
			}
			std::cout << "=================================================" << std::endl;
			mysql_free_result(result);
		}

		sql = "SELECT * FROM `hello`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		result = mysql_use_result(pDB);
		if (result != NULL) {
			show_result(result);
			std::cout << "=================================================" << std::endl;
			mysql_free_result(result);
		}

		sql = "UPDATE `hello` SET age=44 WHERE `编号`=\"9528\";";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "SELECT * FROM `hello`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		result = mysql_use_result(pDB);
		if (result != NULL) {
			show_result(result);
			std::cout << "=================================================" << std::endl;
			mysql_free_result(result);
		}

		sql = "UPDATE `hello` SET age=18 WHERE `age`>35;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		sql = "SELECT * FROM `hello`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		result = mysql_use_result(pDB);
		if (result != NULL) {
			show_result(result);
			std::cout << "=================================================" << std::endl;
			mysql_free_result(result);
		}

		sql = "CREATE INDEX age_index ON hello (age);";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}



		sql = "DELETE FROM `hello` WHERE `编号`=\"9527\";";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "SELECT * FROM `hello`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		result = mysql_use_result(pDB);
		if (result != NULL) {
			show_result(result);
			std::cout << "=================================================" << std::endl;
			mysql_free_result(result);
		}

		sql = "SELECT * FROM `hello` INTO OUTFILE 'C:/ProgramData/MySQL/MySQL Server 8.0/Uploads/hello.txt'\n \
			FIELDS TERMINATED BY ',' \n \
			LINES TERMINATED BY '\r\n'";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());//备份
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "SELECT COUNT(*) FROM `hello`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		result = mysql_use_result(pDB);
		if (result != NULL) {
			show_result(result);
			std::cout << "=================================================" << std::endl;
			mysql_free_result(result);
		}
		sql = "DELETE FROM `hello`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}

		sql = "LOAD DATA LOCAL INFILE 'C:/ProgramData/MySQL/MySQL Server 8.0/Uploads/hello.txt' INTO TABLE `hello`\n \
			FIELDS TERMINATED BY ',' \n \
			LINES TERMINATED BY '\r\n'";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());//恢复
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			std::string cmd = "mysqlimport.exe -u root -pwang410726 --local hello ";
			cmd += "\"C:/ProgramData/MySQL/MySQL Server 8.0/Uploads/hello.txt\" ";
			cmd += "--fields-terminated-by=\",\"  --lines-terminated-by=\"\\r\\n\"";
			system(cmd.c_str());
		}


		sql = "DROP TABLE `hello`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		sql = "DROP TABLE `teacher`;";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(pDB) << std::endl;
			return -2;
		}
		sql = "DROP DATABASE hello; ";
		ret = mysql_real_query(pDB, sql.c_str(), sql.size());
		if (ret != 0) {
			std::cout << "mysql error: " << mysql_error(mysql) << std::endl;
			return -2;
		}
		mysql_close(mysql);
	}
	delete mysql;
	return 0;
}
