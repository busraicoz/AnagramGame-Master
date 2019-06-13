#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#define TABLE_SIZE 8493 //Table_size=(WordNumber / LoadFactor). Word number is 5096 and load foctor is 0.6
#define NO_OF_CHARS 256
 
struct HashTable* hashTable[TABLE_SIZE];
struct HashTable* wordMap[TABLE_SIZE];
char* randomWord;
int sum=0;

int hornerMethod(char* wordArray)
{	int i;
    int result =0;
    for (i = 3; i >= 0; i--)
	{
        result+= wordArray[i]*pow(31,3-i);   			
	}
    return result;
}

//h(key, i) = (h1(key) + i*h2(key)) % m 
//h1(key) = key % m  
//h2(key) = 1 + (key % m2)  (m2=m-1)

int doubleHashing(int key, int index)
{
	return (key%TABLE_SIZE + index*(1+(key%(TABLE_SIZE-1))))%TABLE_SIZE;
	
}

struct HashTable
{
	char value[5];
	int key;

};

void insertHashTable(int key, char* value, struct HashTable** hashMap)
{
	int i = 0;
	struct HashTable* hash = (struct HashTable*)malloc(sizeof(struct HashTable));
	strncpy(hash, value, 4);
	hash->value[4] = '\0';
	hash->key = key;

	int hashKey = doubleHashing(key, i);

	while (hashMap[hashKey] != NULL)
	{
		hashKey = doubleHashing(key, i++);
	}
	hashMap[hashKey] = hash;
}

int searchHashTable(char* key, struct HashTable** hashMap)
{
	int i = 0;
	int hashKey = hornerMethod(key);
	int hashingIndex = doubleHashing(hashKey, i);
	while (hashMap[hashingIndex] != NULL && strncmp(hashMap[hashingIndex]->value, key, 4) != 0)
	{
		hashingIndex = doubleHashing(hashKey, i++);
	}
	if (hashMap[hashingIndex] != NULL && strncmp(hashMap[hashingIndex]->value, key, 4) == 0)
	{
		return i + 1;
	}
	return -1;
}

void generateHashTable()
{
	char wordArray[6];
	FILE* fileReader = fopen("input.txt", "r");
	while (fgets(wordArray, 6, fileReader) != NULL)
	{
		if (wordArray[0] != '\n')
		{
			insertHashTable(hornerMethod(wordArray), wordArray, hashTable);
		}
	}
}

/* function to check whether two strings are anagram and check have been located in hash table*/
bool areAnagram(char* randomWord, char* enteredWord) 
{  
    int count_randomWord[NO_OF_CHARS] = { 0 }; 
    int count_enteredWord[NO_OF_CHARS] = { 0 }; 
    int i; 
  
    for (i = 0; randomWord[i] && enteredWord[i]; i++) { 
        count_randomWord[randomWord[i]]++; 
        count_enteredWord[enteredWord[i]]++; 
    } 
    if(strcmp(randomWord, enteredWord)==0)
   		return false;
    if (randomWord[i] || enteredWord[i]) 
        return false; 
    if(randomWord==enteredWord)
        return false;
    for (i = 0; i < NO_OF_CHARS; i++) 
        if (count_randomWord[i] != count_enteredWord[i]) 
            return false; 
    return true; 
} 

int checkAnagram(char* enteredWord){
	if(areAnagram(randomWord,enteredWord)){
		if(searchHashTable(enteredWord, hashTable) > 0)
	        sum=sum+5;
	    else
	        sum=sum-5;
	}
	else
	    sum=sum-5;
	return sum;
}
int play(){
	int score=0;
	int exit = '0';
	char word[5];
	generateHashTable();
	srand(time(0));
	int random = rand() % TABLE_SIZE;
	while (hashTable[random] == NULL)
	{
		random = rand() % TABLE_SIZE;
	}
	randomWord = hashTable[random];
	printf("Random Word: %s\n", randomWord);
	printf("Anagram Word: ");
	while (exit != 0)
	{
	    scanf("%s", word);
		score = checkAnagram(word);
		printf("\nTotal Score: %d\n", score);
		printf("\nIf do you want exit please enter '0'\n");
		printf("\nIf do you want play new  Anagram Game enter '1'\n");
		printf("\nIf do you want continue enter Anagram Word:\n");
		scanf("%d", &exit);
		if(exit==1){
			printf("\nTotal Score: %d\n", score);
		    sum=0;
	   		play();
		}	    	
	}
	printf("Total Score: %d",score);
}

void main()
{
	play();

}

