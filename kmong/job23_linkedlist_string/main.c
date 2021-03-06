#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char s[1002], w[1002];
char query[55];
int ID;

typedef struct Alpha{  // alphabet list "node"
	int al; 
	struct Alpha *next; 
	struct String* front;
} Alpha;

typedef struct String{ // string list "node"
	int id;
	char str[200];
	struct String *next;
}String;

typedef struct { Alpha* front; }List; //List that I access

int prec(char c){
	if ('a' <=c && c<= 'z' ) return c-'a';
	else if ( 'A' <= c && c<= 'Z' )  return c-'A'+26;
	else return 52;
}

char retr(int n){
	if (0 <=n && n<= 25 ) return n+'a';
	else if ( 26 <= n && n<= 51 )  return n+'A'-26;
	else return '.';
}

List* CreateList(){
	List* list = (List*) malloc(sizeof(List));
	list->front=0;
	return list;
}

Alpha* CreateAlpha(char c){
	Alpha* alpha = (Alpha*) malloc(sizeof(Alpha));
	alpha->al=c; alpha->next=0; alpha->front=0;
	return alpha;
}

String* CreateString(char* str){
	String* string = (String*)malloc(sizeof(String));
	strcpy(string->str, str);
	string->id =ID; 
	string->next=0;
	return string;
}

void pushString(Alpha* alpha, char* str, int mode){ // push String -> to alpha
	String* newStr = CreateString(str); // ID is global var
	String* now = alpha->front;
	String* before=0;

	if( mode>=0) newStr->id = mode;

	if (now == 0){
		alpha->front = newStr;
		return;
	}

	while(now){

		if (!strcmp(now->str,str)) { // if there exist same ...
			free(newStr); // ID -- 
			if(mode<0) ID--;
			return;
		}
		before=now;
		now= now->next;
	}
	before->next = newStr; // append it at end

}

void push(List* list, char* str, int mode){
	// push string -> in to List "Whole"

	Alpha* alpha = list->front;
	Alpha* before=0;
	int chk=0; // chk variable for 

	int al = prec(str[0]); // precalc 

	if ( alpha == 0){ // if list is empty
		Alpha* newAlp =  CreateAlpha(al); 
		list->front = newAlp;
		pushString(newAlp, str, mode); // we push string 
		return; 
	}

	while(alpha){ // if list exist 

		if( alpha->al == al ){   // alpha has same order
			pushString(alpha, str, mode); //push It
			chk=1;
			break;
		}
		else if ( alpha->al  > al ){ // small order
			Alpha* newAlp =  CreateAlpha(al);
			pushString(newAlp, str, mode);

			if(before) before->next = newAlp; // if before exist
			else list->front = newAlp;  // it means str[0] is most small char

			newAlp->next = alpha;
			chk=1;
			break;
		}
		else{
			before=alpha; 
			alpha = alpha->next; //index alpha node move
		}
	}

	if (!chk){ //  biggest order
		Alpha* newAlp =  CreateAlpha(al);
		before->next = newAlp;
		pushString(newAlp, str, mode);
	}
}

void Sch(List* list, char* str){
	Alpha* nowAlp = list->front;
	int al = prec(str[0]);

	while(nowAlp){

		if(nowAlp->al == al ) {
			String* nowStr = nowAlp->front;

			while(nowStr){
				printf("%d", nowStr->id);
				if (!strcmp(nowStr->str,str)) break;
				if(nowStr->next) printf(" ");
				nowStr = nowStr->next;
			}
		}
		nowAlp = nowAlp->next;
	}
}

void Del(List* list, char* str){
	Alpha* nowAlp = list->front;
	int al = prec(str[0]);

	while(nowAlp){

		if ( nowAlp->al == al){
			String* nowStr = nowAlp->front;
			String* before=0;

			while(nowStr){

				if(!strcmp(nowStr->str, str)){

					if(before) before->next= nowStr->next;
					else nowAlp->front = nowStr->next;

					free(nowStr);
					return;

				}

				before=nowStr;
				nowStr=nowStr->next;
			}
		}
		nowAlp = nowAlp->next;
	}
}

void Sub(List* list, char* fstr, char* tstr){
	Alpha* nowAlp = list->front;
	int al = prec(fstr[0]);

	while(nowAlp){

		if ( nowAlp->al == al ){
			
			String* nowStr = nowAlp->front;

			while(nowStr){

				if( !strcmp(nowStr->str, fstr) ){ // we find that fstr!

					push(list, tstr, nowStr->id); // mode >=0 (fstr->id)
					Del(list, fstr);
					return;
				} 
				nowStr = nowStr->next;
			}
		}
		nowAlp  = nowAlp->next;
	}

}

void printAll(List* list){
	Alpha* nowAlp = list->front;

	while(nowAlp){
		printf("nowAlp: %c\n", retr(nowAlp->al));

		String* nowStr = nowAlp->front;

		while(nowStr){

			printf("%s %d\n", nowStr->str, nowStr->id);
			nowStr= nowStr->next;
		}

		nowAlp= nowAlp->next;
		printf("\n");
	}
}

int main(){

	List* list = CreateList();
	fgets(s, 1002,stdin);
	int len = strlen(s);
	if (s[len-1] == '\n') s[--len]=0;
	int start=1, wlen=0, wchk=0;

	for (int i=0; i<len; i++){
		if (s[i] == ' ' ){
			if(wchk){
				w[wlen++]=0; 
				push(list, w, -1); // mode -1  totally new string
				ID++;
				wlen=0, wchk=0;
			}
		}
		else w[wlen++]=s[i], wchk=1; 
	}

	if(wchk){
		w[wlen++]=0; 
		push(list, w, -1); // mode -1
	}

	printAll(list);

	while(scanf("%s", query)){
		if (query[0] == '0') break;
		if ( !strcmp(query, "sub")){

			char t[200];
			scanf("%s %s", w, t);
			Sub(list, w, t);

		}
		else if (!strcmp(query, "sch")){
			scanf("%s", w);
		  Sch(list, w);
			printf("\n");
		}
		else if (!strcmp(query, "del")){
			scanf("%s", w);
			Del(list, w);
		}
	}

}
