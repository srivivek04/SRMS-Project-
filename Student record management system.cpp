#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUDENT_FILE "students.txt"
#define CREDENTIAL_FILE "credentials.txt"

/* ---- ROLES ---- */
#define ROLE_ADMIN 1
#define ROLE_STAFF 2
#define ROLE_GUEST 3

int userRole = 0;  // will be set after login

struct Student
{
    int roll;
    char name[50];
    float marks;
};

/* ------------------- FUNCTION DECLARATIONS ------------------- */
void addStudent();
void displayStudents();
int loginSystem();
void SearchStudent();
void DeleteStudent();
void UpdateStudent();

/* ------------------- MAIN FUNCTION ------------------- */
int main()
{
    if (!loginSystem())
    {
        printf("\nLogin Failed. Exiting...\n");
        return 0;
    }

    int choice;
    do
    {
        printf("\n===== STUDENT MANAGEMENT SYSTEM =====\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Delete Student\n");
        printf("5. Update Student\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            if (userRole == ROLE_GUEST)
            {
                printf("Guests are not allowed to add students.\n");
            }
            else
            {
                addStudent();
            }
            break;

        case 2:
            displayStudents();
            break;

        case 3:
            if (userRole == ROLE_GUEST)
            {
                printf("Guests are not allowed to search students.\n");
            }
            else
            {
                SearchStudent();
            }
            break;

        case 4:
            if (userRole == ROLE_ADMIN)
            {
                DeleteStudent();
            }
            else
            {
                printf("Only admin can delete student records.\n");
            }
            break;

        case 5:
            if (userRole == ROLE_GUEST)
            {
                printf("Guests are not allowed to update students.\n");
            }
            else
            {
                UpdateStudent();
            }
            break;

        case 6:
            printf("Exiting Program...\n");
            break;

        default:
            printf("Invalid Choice! Try Again.\n");
        }
    } while (choice != 6);

    return 0;
}

/* ------------------- LOGIN SYSTEM ------------------- */
/*
   credentials.txt format (one per line):
   username password role

   Example (this program will create it if not present):

   admin admin123 admin
   staff staff123 staff
   guest guest123 guest
*/
int loginSystem()
{
    FILE *file = fopen(CREDENTIAL_FILE, "r");

    if (file == NULL)
    {
        file = fopen(CREDENTIAL_FILE, "w");
        if (file == NULL)
        {
            printf("Error creating credential file.\n");
            return 0;
        }
        fprintf(file, "admin admin123 admin\n");
        fprintf(file, "staff staff123 staff\n");
        fprintf(file, "guest guest123 guest\n");
        fclose(file);

        printf("\nCredential file not found — created default credentials:\n");
        printf("Admin  -> Username: admin  Password: admin123\n");
        printf("Staff  -> Username: staff  Password: staff123\n");
        printf("Guest  -> Username: guest  Password: guest123\n\n");

        file = fopen(CREDENTIAL_FILE, "r");
        if (file == NULL)
        {
            printf("Error opening credential file.\n");
            return 0;
        }
    }

    char inputUser[50], inputPass[50];
    char savedUser[50], savedPass[50], roleStr[20];
    int found = 0;

    printf("\n======== LOGIN SCREEN ========\n");
    printf("Username: ");
    scanf("%s", inputUser);
    printf("Password: ");
    scanf("%s", inputPass);

    while (fscanf(file, "%s %s %s", savedUser, savedPass, roleStr) == 3)
    {
        if (strcmp(inputUser, savedUser) == 0 && strcmp(inputPass, savedPass) == 0)
        {
            found = 1;
            if (strcmp(roleStr, "admin") == 0)
                userRole = ROLE_ADMIN;
            else if (strcmp(roleStr, "staff") == 0)
                userRole = ROLE_STAFF;
            else if (strcmp(roleStr, "guest") == 0)
                userRole = ROLE_GUEST;
            break;
        }
    }
    fclose(file);

    if (found)
    {
        if (userRole == ROLE_ADMIN)
            printf("\nLogin Successful! (Admin)\n");
        else if (userRole == ROLE_STAFF)
            printf("\nLogin Successful! (Staff)\n");
        else if (userRole == ROLE_GUEST)
            printf("\nLogin Successful! (Guest)\n");
        return 1;
    }
    else
    {
        printf("\nInvalid Username or Password!\n");
        return 0;
    }
}

/* ------------------- ADD STUDENT ------------------- */
void addStudent()
{
    struct Student s;
    FILE *file = fopen(STUDENT_FILE, "a");

    if (file == NULL)
    {
        printf("Error opening student file.\n");
        return;
    }

    printf("Enter Roll No: ");
    scanf("%d", &s.roll);
    printf("Enter Name: ");
    getchar();
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = 0;
    printf("Enter Marks: ");
    scanf("%f", &s.marks);

    fprintf(file, "%d %s %.2f\n", s.roll, s.name, s.marks);
    fclose(file);

    printf("\nStudent Record Added Successfully!\n");
}

/* ------------------- DISPLAY STUDENTS ------------------- */
void displayStudents()
{
    struct Student s;
    FILE *file = fopen(STUDENT_FILE, "r");

    if (file == NULL)
    {
        printf("\nNo student records found.\n");
        return;
    }

    printf("\n----- STUDENT RECORDS -----\n");
    while (fscanf(file, "%d %s %f", &s.roll, s.name, &s.marks) != EOF)
    {
        printf("Roll: %d | Name: %s | Marks: %.2f\n", s.roll, s.name, s.marks);
    }
    fclose(file);
}

/* ------------------- SEARCH STUDENT ------------------- */
void SearchStudent()
{
    struct Student s;
    int rollToSearch;
    FILE *file = fopen(STUDENT_FILE, "r");
    if (file == NULL)
    {
        printf("\nNo student records found.\n");
        return;
    }
    printf("\nEnter Roll No to Search: ");
    scanf("%d", &rollToSearch);

    int found = 0;
    while (fscanf(file, "%d %s %f", &s.roll, s.name, &s.marks) != EOF)
    {
        if (s.roll == rollToSearch)
        {
            printf("Roll: %d | Name: %s | Marks: %.2f\n", s.roll, s.name, s.marks);
            found = 1;
            break;
        }
    }
    fclose(file);

    if (!found)
    {
        printf("\nStudent with Roll No %d not found.\n", rollToSearch);
    }
}

/* ------------------- DELETE STUDENT ------------------- */
void DeleteStudent()
{
    struct Student s;
    int rollToDelete;
    FILE *file = fopen(STUDENT_FILE, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (file == NULL)
    {
        printf("\nNo student records found.\n");
        return;
    }
    if (tempFile == NULL)
    {
        printf("\nError creating temp file.\n");
        fclose(file);
        return;
    }

    printf("\nEnter Roll No to Delete: ");
    scanf("%d", &rollToDelete);

    int found = 0;
    while (fscanf(file, "%d %s %f", &s.roll, s.name, &s.marks) != EOF)
    {
        if (s.roll == rollToDelete)
        {
            found = 1;
            continue; // skip this record
        }
        fprintf(tempFile, "%d %s %.2f\n", s.roll, s.name, s.marks);
    }
    fclose(file);
    fclose(tempFile);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
    {
        printf("\nStudent with Roll No %d deleted successfully.\n", rollToDelete);
    }
    else
    {
        printf("\nStudent with Roll No %d not found.\n", rollToDelete);
    }
}

/* ------------------- UPDATE STUDENT ------------------- */
void UpdateStudent()
{
    struct Student s;
    int rollToUpdate;
    FILE *file = fopen(STUDENT_FILE, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (file == NULL)
    {
        printf("\nNo student records found.\n");
        return;
    }
    if (tempFile == NULL)
    {
        printf("\nError creating temp file.\n");
        fclose(file);
        return;
    }

    printf("\nEnter Roll No to Update: ");
    scanf("%d", &rollToUpdate);

    int found = 0;
    while (fscanf(file, "%d %s %f", &s.roll, s.name, &s.marks) != EOF)
    {
        if (s.roll == rollToUpdate)
        {
            found = 1;
            printf("Enter New Name: ");
            getchar();
            fgets(s.name, sizeof(s.name), stdin);
            s.name[strcspn(s.name, "\n")] = 0;
            printf("Enter New Marks: ");
            scanf("%f", &s.marks);
        }
        fprintf(tempFile, "%d %s %.2f\n", s.roll, s.name, s.marks);
    }
    fclose(file);
    fclose(tempFile);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
    {
        printf("\nStudent with Roll No %d updated successfully.\n", rollToUpdate);
    }
    else
    {
        printf("\nStudent with Roll No %d not found.\n", rollToUpdate);
    }
}
