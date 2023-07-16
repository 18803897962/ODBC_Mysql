#define _CRT_SECURE_NO_WARNINGS
#include"CMysql.h"
#include<vector>
//创建mydb数据库
void create_databse() {
	CMysql* m_instance = CMysql::getInstance();
	std::string cmd;

	cmd = "CREATE DATABASE MYDB;";
	m_instance->Query(cmd);
	cmd = "USE MYDB;";
	m_instance->Query(cmd);
}

//创建s349、c349、sc349三个表
void create_tables() {
	CMysql* m_instance = CMysql::getInstance();
	std::string cmd;


	//创建学生表
	cmd = "CREATE TABLE IF NOT EXISTS S349 (\n \
		SNO VARCHAR(8) PRIMARY KEY,\n \
		SNAME VARCHAR(32) NOT NULL,\n \
		SEX VARCHAR(8) NOT NULL,\n \
		BDATE VARCHAR(10) NOT NULL,\n \
		HEIGHT FLOAT NOT NULL,\n \
		DORM CHAR(32) NOT NULL)";
	cmd += " ENGINE = InnoDB DEFAULT CHARSET = utf8; ";
	m_instance->Query(cmd);


	//创建课程表
	cmd = "CREATE TABLE IF NOT EXISTS C349 (\n \
		CNO VARCHAR(8) PRIMARY KEY,\n \
		CNAME VARCHAR(64) NOT NULL,\n \
		PERIOD INT NOT NULL,\n \
		CREDIT INT NOT NULL,\n \
		TEACHER VARCHAR(32) NOT NULL)";
	cmd += " ENGINE = InnoDB DEFAULT CHARSET = utf8; ";
	m_instance->Query(cmd);

	//创建选课表
/*
	cmd = "CREATE TABLE IF NOT EXISTS SC349 (\n \
		SNO VARCHAR(8),\n \
		CNO VARCHAR(8),\n \
		GRADE FLOAT,\n \
		PRIMARY KEY(SNO,CNO),\n \
		FOREIGN KEY(SNO) REFERENCES S349(SNO))";
*/
	cmd = "CREATE TABLE IF NOT EXISTS SC349 (\n \
		SNO VARCHAR(8),\n \
		CNO VARCHAR(8),\n \
		GRADE FLOAT,\n \
		PRIMARY KEY(SNO,CNO),\n \
		CONSTRAINT key_sno FOREIGN KEY(SNO) REFERENCES S349(SNO),\n \
		CONSTRAINT key_cno FOREIGN KEY(CNO) REFERENCES C349(CNO))";
	cmd += " ENGINE = InnoDB DEFAULT CHARSET = utf8; ";
	m_instance->Query(cmd);
}

//原表项内容插入
void insert_info() {
	CMysql* m_instance = CMysql::getInstance();
	std::string cmd;

	cmd = "INSERT INTO S349 (SNO,SNAME,SEX,BDATE,HEIGHT,DORM) VALUES \n \
		('01032010', '王涛', '男', '2002-4-5', 1.72, '东14舍221'),\n \
		('01032023','孙文','男','2003-6-10',1.80,'东14舍221'),\n \
		('01032001', '张晓梅', '女', '2003-11-17', 1.58, '东1舍312'),\n \
		('01032005', '刘静', '女', '2002-1-10', 1.63, '东1舍312'),\n \
		('01032112', '董蔚', '男', '2002-2-20', 1.71, '东14舍221'),\n \
		('01031011', '王倩', '女', '2003-12-20', 1.66, '东2舍104'),\n \
		('01031014', '赵思扬', '男', '2001-6-6', 1.85, '东18舍421'),\n \
		('01031051', '周剑', '男', '2001-5-8', 1.68, '东18舍422'),\n \
		('01031009', '田婷', '女', '2002-8-11', 1.60, '东2舍104'),\n \
		('01031033', '蔡明明', '男', '2002-3-12', 1.75, '东18舍423'),\n \
		('01031056', '曹子衿', '女', '2003-12-15', 1.65, '东2舍305'); ";;
	m_instance->Query(cmd);

	cmd = "INSERT INTO C349 (CNO,CNAME,PERIOD,CREDIT,TEACHER) VALUES \n \
		('CS-01', '数据结构', 60, 3, '张军'),\n \
		('CS-02', '计算机组成原理', 80, 4, '王亚伟'),\n \
		('CS-04', '人工智能', 40, 2, '李蕾'),\n \
		('CS-05', '深度学习', 40, 2, '崔均'),\n \
		('EE-01', '信号与系统', 60, 3, '张明'),\n \
		('EE-02', '数字逻辑电路', 100, 5, '胡海东'),\n \
		('EE-03', '光电子学与光子学', 40, 2, '石韬'); ";
	m_instance->Query(cmd);

	cmd = "INSERT INTO SC349 (SNO,CNO,GRADE) VALUES \n \
		('01032010', 'CS-01', 82.0),\n \
		('01032010', 'CS-02', 91.0),\n \
		('01032010', 'CS-04', 83.5),\n \
		('01032001', 'CS-01', 77.5),\n \
		('01032001', 'CS-02', 85.0),\n \
		('01032001', 'CS-04', 83.0),\n \
		('01032005', 'CS-01', 62.0),\n \
		('01032005', 'CS-02', 77.0),\n \
		('01032005', 'CS-04', 82.0),\n \
		('01032023', 'CS-01', 55.0),\n \
		('01032023', 'CS-02', 81.0),\n \
		('01032023', 'CS-04', 76.0),\n \
		('01032112', 'CS-01', 88.0),\n \
		('01032112', 'CS-02', 91.5),\n \
		('01032112', 'CS-04', 86.0),\n \
		('01032112', 'CS-05', NULL),\n \
		('01031033', 'EE-01', 93.0),\n \
		('01031033', 'EE-02', 89.0),\n \
		('01031009', 'EE-01', 88.0),\n \
		('01031009', 'EE-02', 78.5),\n \
		('01031011', 'EE-01', 91.0),\n \
		('01031011', 'EE-02', 86.0),\n \
		('01031051', 'EE-01', 78.0),\n \
		('01031051', 'EE-02', 58.0),\n \
		('01031014', 'EE-01', 79.0),\n \
		('01031014', 'EE-02', 71.0);";
	m_instance->Query(cmd);
}

//备份与恢复
void backup_recover() {
	CMysql* m_instance = CMysql::getInstance();
	std::string cmd;
	//数据备份
	/*
	cmd = "SELECT * FROM SC349 INTO OUTFILE 'C:/ProgramData/MySQL/MySQL Server 8.0/Uploads/SC349.txt'\n \
			FIELDS TERMINATED BY ',' \n \
			LINES TERMINATED BY '\r\n'";
	m_instance->Query(cmd);*/


	//数据恢复
	cmd = "LOAD DATA LOCAL INFILE 'C:/ProgramData/MySQL/MySQL Server 8.0/Uploads/S349.txt' INTO TABLE `S349`\n \
			FIELDS TERMINATED BY ',' \n \
			LINES TERMINATED BY '\r\n'";
	m_instance->Query(cmd);

	cmd = "LOAD DATA LOCAL INFILE 'C:/ProgramData/MySQL/MySQL Server 8.0/Uploads/C349.txt' INTO TABLE `C349`\n \
			FIELDS TERMINATED BY ',' \n \
			LINES TERMINATED BY '\r\n'";
	m_instance->Query(cmd);

	cmd = "LOAD DATA LOCAL INFILE 'C:/ProgramData/MySQL/MySQL Server 8.0/Uploads/SC349.txt' INTO TABLE `SC349`\n \
			FIELDS TERMINATED BY ',' \n \
			LINES TERMINATED BY '\r\n'";
	m_instance->Query(cmd);
}

//删除数据库
void drop_databse() {
	CMysql* m_instance = CMysql::getInstance();
	std::string cmd;
	cmd = "DROP DATABASE MYDB;";
	m_instance->Query(cmd);
}

//解析s349表的sql插入命令
void parser_student_rows(const std::string& row, std::string& cmd) {
	std::string sno, sname, sex, bdate, height, dorm;
	unsigned int index = 0;
	unsigned int i = 0;
	while (row[i] != ',') i++;
	sno = row.substr(index, i - index);
	i++; index = i;
	while (row[i] != ',') i++;
	sname = row.substr(index, i - index);
	i++; index = i;
	while (row[i] != ',') i++;
	sex = row.substr(index, i - index);
	i++; index = i;
	while (row[i] != ',') i++;
	bdate = row.substr(index, i - index);
	i++; index = i;
	while (row[i] != ',') i++;
	height = row.substr(index, i - index);
	i++; index = i;
	dorm = row.substr(index, row.size() - index);

	cmd = "INSERT INTO S349 (SNO,SNAME,SEX,BDATE,HEIGHT,DORM) VALUES (";
	cmd += "'" + sno + "',";
	cmd += "'" + sname + "',";
	cmd += "'" + sex + "',";
	cmd += "'" + bdate + "',";
	cmd += height + ",";
	cmd += "'" + dorm + "');";
}

//将s349.txt插入s349表
int insert_into_student() {
	CMysql* m_instance = CMysql::getInstance();
	size_t ret = 0;
	std::string cmd;

	//FILE* pfile = fopen("C:\\ProgramData\\MySQL\\MySQL Server 8.0\\Uploads\\s.txt", "rb");
	FILE* pfile = fopen("D:\\ttest\\s.txt", "rb");
	if (pfile == NULL) {
		printf("open file failed\n");
		return -1;
	}

	std::string buffer;
	buffer.resize(4096);
	ret = fseek(pfile, 0, SEEK_END);
	long file_size = ftell(pfile);
	long index = 0;//已经读取的长度
	int left_size = 0;//上次处理剩余长度
	int last_index = 0;//下一个row起始位置
	ret = fseek(pfile, 0, SEEK_SET);
	while (index < file_size) {
		long read_size = file_size - index > (long)buffer.size() - left_size ? (long)buffer.size() - left_size : file_size - index;//缓冲区可读取的大小
		long cur_size = read_size + left_size;//缓冲区数据大小
		ret = fread((void*)(buffer.c_str() + left_size), 1, read_size, pfile);
		if (ret <= 0) {
			printf("read file failed\n");
			return -2;
		}
		index += (long)ret;
		for (int i = 0; i < cur_size; i++) {
			if (buffer[i] == '\r') {
				std::string row = buffer.substr(last_index, i - last_index);
				std::string cmd;
				parser_student_rows(row, cmd);
				ret = m_instance->Query(cmd);
				if (ret != 0) {
					m_instance->ShowError();
					return -1;
				}
				last_index = i + 2;
			}
			if (i == cur_size - 1 && index == file_size) {
				std::string row = buffer.substr(last_index, i - last_index + 1);
				std::string cmd;
				parser_student_rows(row, cmd);
				ret = m_instance->Query(cmd);
				if (ret != 0) {
					m_instance->ShowError();
					return -1;
				}
				last_index = cur_size;
			}
		}
		if (last_index < cur_size) {
			left_size = cur_size - last_index;
			memcpy((void*)buffer.c_str(), (void*)(buffer.c_str() + last_index), left_size);
			last_index = 0;
		}
		else {
			left_size = 0;
			last_index = last_index - cur_size;
		}
	}


	fclose(pfile);

	return 0;
}

//解析sc349的sql插入命令
void parser_course_rows(const std::string& row, std::string& cmd) {
	std::string cno, cname, period, credit, teacher;
	unsigned int index = 0;
	unsigned int i = 0;
	while (row[i] != ',') i++;
	cno = row.substr(index, i - index);
	i++; index = i;
	while (row[i] != ',') i++;
	cname = row.substr(index, i - index);
	i++; index = i;
	while (row[i] != ',') i++;
	period = row.substr(index, i - index);
	i++; index = i;
	while (row[i] != ',') i++;
	credit = row.substr(index, i - index);
	i++; index = i;
	
	teacher = row.substr(index, row.size() - index);

	cmd = "INSERT INTO C349 (CNO,CNAME,PERIOD,CREDIT,TEACHER) VALUES (";
	cmd += "'" + cno + "',";
	cmd += "'" + cname + "',";
	cmd += period + ",";
	cmd += credit + ",";
	cmd += "'" + teacher + "');";
}

//将c349.txt插入c349表
int insert_into_course() {
	CMysql* m_instance = CMysql::getInstance();
	size_t ret = 0;
	std::string cmd;

	//FILE* pfile = fopen("C:\\ProgramData\\MySQL\\MySQL Server 8.0\\Uploads\\C349.txt", "rb");  //不同表单的不同点 1
	FILE* pfile = fopen("D:\\ttest\\c.txt", "rb");  //不同表单的不同点 1

	if (pfile == NULL) {
		printf("open file failed\n");
		return -1;
	}

	std::string buffer;
	buffer.resize(4096);
	ret = fseek(pfile, 0, SEEK_END);
	long file_size = ftell(pfile);
	long index = 0;//已经读取的长度
	int left_size = 0;//上次处理剩余长度
	int last_index = 0;//下一个row起始位置
	ret = fseek(pfile, 0, SEEK_SET);
	while (index < file_size) {
		long read_size = file_size - index > (long)buffer.size() - left_size ? (long)buffer.size() - left_size : file_size - index;//缓冲区可读取的大小
		long cur_size = read_size + left_size;//缓冲区数据大小
		ret = fread((void*)(buffer.c_str() + left_size), 1, read_size, pfile);
		if (ret <= 0) {
			printf("read file failed\n");
			return -2;
		}
		index += (long)ret;
		for (int i = 0; i < cur_size; i++) {
			if (buffer[i] == '\r') {
				std::string row = buffer.substr(last_index, i - last_index);
				std::string cmd;
				parser_course_rows(row, cmd);   //不同表单的不同点 2
				ret = m_instance->Query(cmd); 
				if (ret != 0) {
					m_instance->ShowError();
					return -1;
				}
				last_index = i + 2;
			}
			if (i == cur_size - 1 && index == file_size) {
				std::string row = buffer.substr(last_index, i - last_index + 1);
				std::string cmd;
				parser_course_rows(row, cmd);   //不同表单的不同点 3
				ret = m_instance->Query(cmd);
				if (ret != 0) {
					m_instance->ShowError();
					return -1;
				}
				last_index = cur_size;
			}
		}
		if (last_index < cur_size) {
			left_size = cur_size - last_index;
			memcpy((void*)buffer.c_str(), (void*)(buffer.c_str() + last_index), left_size);
			last_index = 0;
		}
		else {
			left_size = 0;
			last_index = last_index - cur_size;
		}
	}


	fclose(pfile);
	return 0;
}

//解析sc349表的sql插入命令
void parser_sc_rows(const std::string& row, std::string& cmd) {
	std::string sno, cno, grade;
	unsigned int index = 0;
	unsigned int i = 0;
	while (row[i] != ',') i++;
	sno = row.substr(index, i - index);
	i++; index = i;
	while (row[i] != ',') i++;
	cno = row.substr(index, i - index);
	i++; index = i;
	grade = row.substr(index, row.size() - index);
	if (grade == "\\N") {
		grade = "NULL";
	}

	cmd = "INSERT INTO SC349 (SNO,CNO,GRADE) VALUES (";
	cmd += "'" + sno + "',";
	cmd += "'" + cno + "',";
	cmd += grade + ");";
}

//将sc349.txt插入sc349表
int insert_into_sc() {
	CMysql* m_instance = CMysql::getInstance();
	size_t ret = 0;
	std::string cmd;

	//FILE* pfile = fopen("C:\\ProgramData\\MySQL\\MySQL Server 8.0\\Uploads\\SC349.txt", "rb");
	FILE* pfile = fopen("D:\\ttest\\sc.txt", "rb");

	if (pfile == NULL) {
		printf("open file failed\n");
		return -1;
	}

	std::string buffer;
	buffer.resize(4096);
	ret = fseek(pfile, 0, SEEK_END);
	long file_size = ftell(pfile);
	long index = 0;//已经读取的长度
	int left_size = 0;//上次处理剩余长度
	int last_index = 0;//下一个row起始位置
	ret = fseek(pfile, 0, SEEK_SET);
	while (index < file_size) {
		long read_size = file_size - index > (long)buffer.size() - left_size ? (long)buffer.size() - left_size : file_size - index;//缓冲区可读取的大小
		long cur_size = read_size + left_size;//缓冲区数据大小
		ret = fread((void*)(buffer.c_str() + left_size), 1, read_size, pfile);
		if (ret <= 0) {
			printf("read file failed\n");
			return -2;
		}
		index += (long)ret;
		for (int i = 0; i < cur_size; i++) {
			if (buffer[i] == '\r') {
				std::string row = buffer.substr(last_index, i - last_index);
				std::string cmd;
				parser_sc_rows(row, cmd);
				ret = m_instance->Query(cmd);
				if (ret != 0) {
					m_instance->ShowError();
					return -1;
				}
				last_index = i + 2;
			}
			if (i == cur_size - 1 && index == file_size) {
				std::string row = buffer.substr(last_index, i - last_index + 1);
				std::string cmd;
				parser_sc_rows(row, cmd);
				ret = m_instance->Query(cmd);
				if (ret != 0) {
					m_instance->ShowError();
					return -1;
				}
				last_index = cur_size;
			}
		}
		if (last_index < cur_size) {
			left_size = cur_size - last_index;
			memcpy((void*)buffer.c_str(), (void*)(buffer.c_str() + last_index), left_size);
			last_index = 0;
		}
		else {
			left_size = 0;
			last_index = last_index - cur_size;
		}
	}


	fclose(pfile);




	return 0;
}

//随机删除200行分数小于60的数据
int random_delete() {
	CMysql* m_instance = CMysql::getInstance();
	int ret = 0;
	std::string cmd;

	cmd = "SELECT SNO,CNO FROM SC349 WHERE GRADE < 60";
	m_instance->Query(cmd);


	//获取表行和列的相关信息
	MYSQL_RES* result = mysql_store_result(*m_instance);
	if (result == NULL) {
		m_instance->ShowError("show result failed");
		return -1;
	}
	unsigned int nFields = mysql_num_fields(result);
	my_ulonglong nRows = mysql_num_rows(result);

	//获取200个随机下标
	std::vector<int> indexs;
	std::vector<std::string> sno_and_cnos;
	while (indexs.size() < 200) {
		int num = rand() % nRows;
		if (std::find(indexs.begin(), indexs.end(), num) != indexs.end()) continue;
		indexs.push_back(num);
	}

	//获取到需要删除的信息的sno和cno
	MYSQL_FIELD* fields = mysql_fetch_fields(result);
	for (unsigned i = 0; i < nRows; i++) {
		MYSQL_ROW row = mysql_fetch_row(result);
		if (std::find(indexs.begin(), indexs.end(), i) != indexs.end()) {
			std::string sno_and_cno;
			for (unsigned j = 0; j < nFields; j++) {
				sno_and_cno += row[j];
			}
			sno_and_cnos.emplace_back(sno_and_cno);
		}
	}

	for (size_t i = 0; i < sno_and_cnos.size(); i++) {
		cmd = "DELETE FROM SC349 WHERE SNO = '";
		cmd += sno_and_cnos[i].substr(0, 8);
		cmd += "' AND CNO = '";
		cmd += sno_and_cnos[i].substr(8, sno_and_cnos[i].size() - 8);
		cmd += "';";
		m_instance->Query(cmd);
	}
	return 0;
}

int main()
{
	CMysql* m_instance = CMysql::getInstance();
	int ret = 0;
	std::string cmd;
	ret = m_instance->Login(true);
	ret = m_instance->Set_EncodingFormat("utf8");
	
	cmd = "USE MYDB;";
	m_instance->Query(cmd);

	//insert_info();

	//create_databse();
	//create_tables();

	//ret = insert_into_student();
	//ret = insert_into_course();
	//ret = insert_into_sc();

	//ret = random_delete();

	/*
	cmd = "SELECT * FROM SC349 INTO OUTFILE 'C:/ProgramData/MySQL/MySQL Server 8.0/Uploads/backup_SC349.txt'\n \
			FIELDS TERMINATED BY ',' \n \
			LINES TERMINATED BY '\r\n'";
	m_instance->Query(cmd);	
	*/
	/*
	cmd = "LOAD DATA LOCAL INFILE 'C:/ProgramData/MySQL/MySQL Server 8.0/Uploads/sc.txt' \n \
			INTO TABLE `SC847`\n \
			FIELDS TERMINATED BY ',' \n \
			LINES TERMINATED BY '\r\n'";
	m_instance->Query(cmd);
	*/
	return 0;
}