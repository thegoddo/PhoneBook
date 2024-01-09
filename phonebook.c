#include <ctype.h>
#include <ncurses.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int callback(void* data, int argc, char** argv, char** azColName) {
  int i;
  fprintf(stderr, "%s: ",(const char*)data);

  for(int i = 0; i < argc; i++) {
    printf("%s = %s\n", azColName[i],argv[i] ? argv[i]: "NULL");
  }
  printf("\n");
  return 0;
}

/* Shows the menu screen. */
int menu() {
  int choice;

  printf("PhoneBook Menu");
  printf("\n1.Add\n2.Delete\n3.Show\n4.Exit\n");
  scanf("%d", &choice);

  return choice;
}

/* Checks if the database exist. */
bool check_file() {
  char *file_name = "phonebook.db";
  char file_choice;
  FILE *fptr;

  if (access(file_name, F_OK) == 0) { // checks if file name exist.
  } else {
    printf("Database Does not exist.\n Create a Database?(Y/N): ");
    scanf("%c", &file_choice);

    putchar(toupper(file_choice));

    if (file_choice == 'Y') {
      fptr =
          fopen(file_name, "r+"); // create the file with read and write access.
    } else {
      return 1; // if user chose 'N', exit the program.
    }
  }
  fclose(fptr);
  return 0;
};

/* create table if does not exist. */
bool check_db(sqlite3 *db, int rc) {
  const char *sql = "CREATE TABLE IF NOT EXISTS phonebook ("
                    "ID INT PRIMARY KEY NOT NULL,"
                    "Name TEXT NOT NULL,"
                    "Surname TEXT NOT NULL,"
                    "Number INT NOT NULL);";
  char* message_error;
  rc = sqlite3_exec(db, sql, NULL, 0, &message_error);

  if(rc != SQLITE_OK) {
    fprintf(stderr, "Error creating table");
    sqlite3_free(message_error);

    return 1;
  } 
  return 0;
}

void insert_data(sqlite3 *db, int rc) {

  char* sql = "INSERT INTO phonebook VALUES(1, 'Biswajit', 'Shaw', 8207365857);";
  char* error_message;

  if(check_file()) {
  }else {
    exit(1);
  };
  if(check_db(db, rc)) {
    rc = sqlite3_exec(db, sql, NULL, 0, &error_message);

    if(rc != SQLITE_OK) {
      printf("Insert Error!!\n");
      sqlite3_free(error_message);
    } else {
      printf("Record created successfully.");
    }
  }; // checks if tables exist in database.
}

void drop_data(sqlite3 *db,int rc) {
  if(check_db(db,rc)) {
    char* sql = "DELETE FROM phonebook WHERE Number=8207365857;";
  }
}

void traverse_data(sqlite3 *db,int rc) {
  char* query = "SELECT * FROM phonebook;";
  sqlite3_exec(db, query, callback, NULL, NULL);
}

/* choice handles for menu. */
void choice_handler(int choice, sqlite3 *db, int rc) {

  switch (choice) {
  case 1:
    insert_data(db, rc);
    break;
  case 2:
    drop_data(db, rc);
    break;
  case 3:
    traverse_data(db, rc);
    break;
  case 4:
    return;
    break;
  default:
    printf("Please Choose the valid answers.");
  }
}

void close_sqlite(sqlite3 *db) { sqlite3_close(db); }


/* Main driver. */
int main(int argc, char **argv) {
  sqlite3 *db;
  sqlite3_stmt *res;
  int rc = sqlite3_open("phonebook.db", &db);
  int choice = menu();

  choice_handler(choice, db, rc);
  close_sqlite(db);
  return 0;
}
