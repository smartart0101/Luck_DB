//230605--luck_db 数据库文件起始

#include<stdio.h>

#include<iostream>

using namespace std;

const char *nextLineHeader ="> ";


void printHelpMess();



void printHelpMess(){
	cout << "*********************************************************************************************"<<endl<<endl
		<<" 				Welcome to the duck_db\n 				db file locates in \"./data/db.bin\" \n 				Author: enpei\n 				www.enpeizhao.com\n 				2018-08-31"<<endl<<endl
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
