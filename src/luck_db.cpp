//230605--luck_db 数据库文件起始

#include<stdio.h>
#include<string.h>
#include<iostream>
#include "../include/bpt.h"
#include "../include/TextTable.h"
// #include"predefined.h";

using namespace std;
using namespace bpt;

const char * nextLineHeader = " > ";
const char * exitMessage = "不响丸辣，退出-- quit";
const char * ErrorMessage = "错误的操作 -_- error";
const char *dbFileName = "./data/db.bin";
const char *nextLineHeader ="> ";

clock_t startTime, finishTime;

bplus_tree * luck_db_ptr;

void printHelpMess();
void selectCommand();
void printHelp();


void intiDataBase(){
	cout << " create new database" << endl;

	printHelpMess();


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
				cout << "你不能删除这个文件" << nextLineHeader;
			}else{
				cout << "删除成功" << endl;
			}
			//创建一个新数据库
			intiDataBase();
		}else if(strncmp(userCommand, "insert", 6) == 0){	//确定要执行插入操作
			//插入记录
			int *insertKey = new int;
			value_t *insertData = new value_t;

			//使用sccanf 识别sql 
			int okNum = sscanf(userCommand, "insert db %d %s %d %s", insertKey,
						insertData->name, (&insertData->age), insertData->email);

			//insertKey 自动生成，因此只用判断用户输入的字符串即可.用户需要输入三个信息，小于三个不行	
			if(okNum < 3){
				cout << ErrorMessage << nextLineHeader;
			}else{

				startTime = clock();

				int return_code = insertRecord(luck_db_ptr, insertKey, insertData);

				finishTime = clock();

				if(return_code == 0){ 	//成功插入
					cout << "executed insert index 成功插入数据" << 
					*insertKey << "use time :" << durationTime(&finishTime, &startTime) << nextLineHeader;
				}else if(return_code == 1){
					cout << "failed : 存放位置已经有数据了" << *insertKey << nextLineHeader;
				}else{
					cout << "error\n" << nextLineHeader;
				}
			}
		
		}else if(strncmp(userCommand, "delete", 6) == 0){
			//删除记录
			int *deleteKey = new int;

			int okNum = sscanf(userCommand, "delete from db where id = %d", deleteKey);

			if(okNum < 1){
				cout << ErrorMessage << nextLineHeader;
			}else{
				startTime = clock();
				int return_code = deleteRecord(luck_db_ptr, deleteKey);
				finishTime = clock();

				if(return_code == 0){
					cout << "executed delete删除成功, index:" << *deleteKey << "time :" << 
					durationTime(&finishTime, &startTime) << nextLineHeader;
				}else if(return_code == -1){
					cout << "falied 删除失败, no index:" << deleteKey << "\n" << nextLineHeader;
				}else{
					cout << "error\n" << nextLineHeader;
				}
			}

		}else if(strncmp(userCommand, "usdate", 6) == 0){
			//更新数据
			int *updateIndex = new int;
			value_t *updateData = new value_t;

			int okNum = sscanf(userCommand, "update db %s %d %s where id = %d",
								updateData->name, &(updateData->age), updateData->email); 
			
			if(okNum < 3){	//数据不够，
				cout << ErrorMessage << nextLineHeader;
			}else{
				startTime = clock();

				int return_code = updateRecord(luck_db_ptr, updateIndex, updateData);

				finishTime = clock();
				
				if(return_code == 0){
					cout << "excuted update index:" << updateIndex << "time:"<<
					durationTime(&finishTime, &startTime) <<"seconds"<< nextLineHeader;
				}else{
					cout << "can not update, no index:" << updateIndex << "time:"<<
					durationTime(&finishTime, &startTime) <<"seconds"<< nextLineHeader;
				}
			}
		}else if(strncmp(userCommand, "select", 6) == 0){
			//查找数据
			if(strstr(userCommand, "=") != nullptr){	//strstr 返回第二个字符串在第一个字符串的指针，或nullptr
				int i_start = 0, i_end = 0;		//初始化查询id的起始范围

				int okNum = sscanf(userCommand, "select * from db where id in (%d, %d)",
									i_start, i_end);
				
				if(okNum < 2){	//查询id不够
					cout << ErrorMessage <<nextLineHeader;
				}else{
					startTime = clock();

					selectAll(luck_db_ptr, &i_start, &i_end);

					finishTime = clock();
				}
			}else{

				int *SelectKey = new int;
				int okNum = sscanf(userCommand, "select * from db shere id = %d", SelectKey);

				if(okNum < 1){
					cout<< "can not find key:" << nextLineHeader;
				}else{

					value_t *return_val = new value_t;

					startTime = clock();

					int return_code = searchRecord(luck_db_ptr, SelectKey, return_val);

					finishTime = clock();

					if (return_code != 0){
						cout << "> index:"<< *SelectKey << " doesn't exist, time : "<< durationTime(&finishTime,&startTime) <<" seconds\n"<< nextLineHeader;
					}else{
						printTable( SelectKey , return_val);
						cout << "> executed search, time : "<< durationTime(&finishTime,&startTime) <<" seconds\n"<< nextLineHeader;
						
					}
				}

			}
		}else{
			cout << ErrorMessage << endl;
		}


	}
	
}


void printHelp(){
	cout << "want help? okay!" << endl;
}

//自定义方法，将自动生成的索引 index，用户输入的数据，储存到 b+tree 数据结构中去.返回值是
int insertRecord(bplus_tree *treePtr, int *index, value_t *values){
	bpt :: key_t key;
	//把数字传给索引key
	intToKeyT(&key, index);
	//insrt 函数(bpt.cpp里面定义)，想一个容器中插入元素，容器.insert(位置, 数据);
	return (*treePtr).insert(key, *values);
}

int deleteRecord(bplus_tree *treePtr, int *index){
	bpt::key_t key;
	intToKeyT(&key, index);
	return (*treePtr).remove(key);
}

int updateRecord(bplus_tree *treePtr, int * index, value_t * value){
	bpt::key_t key;
	intToKeyT(&key, index);
	return (*treePtr).update(key, *value);
}

//select all
void selectAll(bplus_tree * treePtr, int * i_start, int * i_end){
	
	TextTable t('-', '|', '+');

	t.add("id");
	t.add("name");
	t.add("age");
	t.add("email");
	t.endOfRow();

	bpt::key_t key;
	value_t *return_val = new value_t;

	for(int i = *i_start; i < *i_end; i++){

		intToKeyT(&key, &i);

		int return_code = (*treePtr).search(key, return_val);

		switch(return_code){
			case -1:
				//不存在，
				break;
			case 0:
				//找到了
				t.add(to_string(i));
				t.add(return_val->name);
				t.add(to_string(return_val->age));
				t.add(return_val->email);
				t.endOfRow();
				break;
			case 1:
				//已经被删除了
				break;
		}
	}

}

int searchRecord(bplus_tree *treeptr, int *index, value_t *return_val){
	bpt::key_t key;
	intToKeyT(&key, index);
	return (*treeptr).search(key, return_val);
}

void intToKeyT(bpt::key_t * a, int *b){
	char key[16] = {0};
	sprintf(key, "%d", *b);	//将格式化的数据写入字符串
	*a = key;
}

double durationTime(clock_t *startTime, clock_t *finishTime){
	return (double)(*startTime - *finishTime) / CLOCKS_PER_SEC;	//通常将clock计算得到的数字除以CLOCKS_PER_SEC得到具体时间
}

void printTable(int *index, value_t *value){

	
}




int main(int argc, char *argv[]){
	intiDataBase();
}