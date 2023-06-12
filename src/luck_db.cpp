//230605--luck_db 数据库文件起始

#include<stdio.h>
#include<string.h>
#include<iostream>

using namespace std;

const char * nextLineHeader = " > ";
const char * exitMessage = "不响丸辣，退出-- quit";
const char * ErrorMessage = "错误的操作---";
const char *dbFileName = "./data/db.bin";

void printHelpMess();
void selectCommand();
void printHelp();


void intiDataBase(){
	cout << " create new database" << endl;
}

void printHelpMess(){
	cout << "*********************************************************************************************"<<endl<<endl
		<<" 				Welcome to the luck_db\n 				"<<endl<<endl
		<<"*********************************************************************************************"<<endl
		<<"  .help 							print help message;"<<endl
		<<"  .exit 							exit program;"<<endl
		<<"  .reset 							delete db file;"<<endl
		<<"  insert db {index} {name} {age} {email}; 			insert record;"<<endl
		<<"  delete from db where id ={index}; 				delete record;"<<endl
		<<"  update db {name} {age} {email} where id={index}; 		update a record;"<<endl
		<<"  select * from db where id={index}; 				search a record by index;"<<endl
		<<"  select * from db where id in({minIndex},{maxIndex}); 		search records between indexs;"<<endl
		<<"*********************************************************************************************"<<endl
		<<endl << nextLineHeader;
}

//selectCommand，用于判断用户输入的是什么，并作出相应的响应

void selectCommand(){

	char * userCommand = new char[256];

	//读输入的字符串，成功返回字节数，失败-1
	cin.getline(userCommand, 256);

	while(true){

		if(strcmp(userCommand, ".exit") == 0){
			//要退出
			cout << exitMessage;
			break;
		}else if(strcmp(userCommand, ".help") == 0){
			//需要帮助界面
			printHelp();
		}else if(strcmp(userCommand, ".reset")){
			//重置数据库，先判断一下是否有这个数据库，有的话就删除并重建，没有的话就提示一句 ，并新建一个
			if(remove(dbFileName) != 0){	//将删除./data/db.bin目录下所有的文件,成功返回0
				cout << "你不能删除这个文件" << endl;
			}else{
				cout << "删除成功" << endl;
			}
			//创建一个新数据库
			intiDataBase();
		}else if(strncmp(userCommand, "insert", 6) == 0){
			//插入记录
		}else if(strncmp(userCommand, "delete", 6) == 0){
			//删除记录
		}else if(strncmp(userCommand, "usdate", 6) == 0){
			//更新数据
		}else if(strncmp(userCommand, "select", 6) == 0){
			//查找数据
		}else{
			cout << ErrorMessage << endl;
		}


	}
	
}


void printHelp(){
	cout << "help? noway" << endl;
}
