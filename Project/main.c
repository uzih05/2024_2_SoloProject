//202410288 유지헌 개인 프로젝트 과제

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "학생정보파일.txt"
#define TEMP_FILENAME "임시파일.txt"

// 학생 정보를 저장할 구조체 정의
typedef struct {
    int id;
    char name[20];
    char department[20];
    char email[30];
    char phone[15];
} Student;

// 학생 정보 불러오기 함수
int load_students(Student *students) {
    FILE *file = fopen(FILENAME, "r");
    int count = 0;

    if (file == NULL) return 0;

    char line[200];
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%d,%[^,],%[^,],%[^,],%s", &students[count].id, students[count].name,
               students[count].department, students[count].email, students[count].phone);
        count++;
    }
    fclose(file);
    return count;
}

// 학생 정보 저장 함수
void save_students(Student *students, int count) {
    FILE *file = fopen(FILENAME, "w");
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d,%s,%s,%s,%s\n", students[i].id, students[i].name, students[i].department,
                students[i].email, students[i].phone);
    }
    fclose(file);
}

// 학생 정보 추가 함수
void add_student() {
    Student student;
    Student students[100];

    int count = load_students(students);

    printf("학번 입력: ");
    scanf("%d", &student.id);  // 학번을 int로 받음
    printf("이름 입력: ");
    scanf("%s", student.name);
    printf("학과 입력: ");
    scanf("%s", student.department);
    printf("이메일 입력: ");
    scanf("%s", student.email);
    printf("전화번호 입력( - 제외): ");
    scanf("%s", student.phone);

    students[count++] = student;

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (students[i].id > students[j].id) {
                Student temp = students[i];
                students[i] = students[j];
                students[j] = temp;
            }
        }
    }

    save_students(students, count);
    printf("학생 정보가 추가되고 정렬되었습니다.\n");
}

// 학생 정보 검색 함수
void search_student() {
    FILE *file = fopen(FILENAME, "r");
    int search_type;

    if (file == NULL) {
        perror("파일 열기 오류");
        return;
    }

    while (1) {
        printf("1. 학번으로 검색\n2. 학과로 검색\n검색 방법 선택: ");
        if (scanf("%d", &search_type) != 1) {
            printf("잘못된 입력입니다. 1 또는 2를 선택해 주세요.\n");
            while (getchar() != '\n');
            continue;
        }
        if (search_type != 1 && search_type != 2) {
            printf("잘못된 입력입니다. 1 또는 2를 선택해 주세요.\n");
            continue;
        }
        break;
    }

    char search_term[20];
    int search_id = 0;

    if (search_type == 1) {
        printf("검색할 학번 입력: ");
        scanf("%d", &search_id);
    } else {
        printf("검색할 학과 입력: ");
        scanf("%s", search_term);
    }

    char line[200];
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        Student student;
        sscanf(line, "%d,%[^,],%[^,],%[^,],%s", &student.id, student.name, student.department, student.email, student.phone);

        if ((search_type == 1 && student.id == search_id) ||
            (search_type == 2 && strcmp(student.department, search_term) == 0)) {
            printf("학번: %d, 이름: %s, 학과: %s, 이메일: %s, 전화번호: %s\n",
                   student.id, student.name, student.department, student.email, student.phone);
            found = 1;
        }
    }

    if (!found) {
        printf("검색 결과가 없습니다.\n");
    }

    fclose(file);
}

// 학생 정보 내보내기 함수
void export_students() {
    FILE *file = fopen(FILENAME, "r");
    char search_term[20];
    int export_type;

    if (file == NULL) {
        perror("파일 열기 오류");
        return;
    }

    printf("1. 전체 내보내기\n2. 특정 학번 내보내기\n3. 특정 학과 내보내기\n내보내기 방법 선택: ");
    scanf("%d", &export_type);

    if (export_type == 2 || export_type == 3) {
        printf("검색어 입력: ");
        scanf("%s", search_term);
    }

    char line[200];
    char filename[50];
    FILE *output_file = NULL;

    if (export_type == 1) {
        strcpy(filename, "전체학생정보.txt");
        output_file = fopen(filename, "w");
    } else if (export_type == 2) {
        sprintf(filename, "학번_%s_학생정보.txt", search_term);
        output_file = fopen(filename, "w");
    } else if (export_type == 3) {
        sprintf(filename, "학과_%s_학생정보.txt", search_term);
        output_file = fopen(filename, "w");
    }

    if (output_file == NULL) {
        perror("파일 작성 오류");
        fclose(file);
        return;
    }

    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        Student student;
        sscanf(line, "%d,%[^,],%[^,],%[^,],%s", &student.id, student.name, student.department, student.email, student.phone);

        char id_str[20];
        sprintf(id_str, "%d", student.id);

        if (export_type == 1 ||
            (export_type == 2 && strcmp(id_str, search_term) == 0) ||
            (export_type == 3 && strcmp(student.department, search_term) == 0)) {
            fprintf(output_file, "학번: %d, 이름: %s, 학과: %s, 이메일: %s, 전화번호: %s\n",
                    student.id, student.name, student.department, student.email, student.phone);
            found = 1;
        }
    }

    if (!found && (export_type == 2 || export_type == 3)) {
        printf("내보낼 정보가 없습니다.\n");
    } else {
        printf("%s에 내보내기 완료되었습니다.\n", filename);
    }

    fclose(file);
    fclose(output_file);
}

// 학생 정보 수정 함수
void edit_student() {
    int edit_id;
    printf("수정할 학번 입력: ");
    scanf("%d", &edit_id);

    Student students[100];
    int count = load_students(students);
    int found = 0;

    for (int i = 0; i < count; i++) {
        if (students[i].id == edit_id) {
            found = 1;
            int edit_choice;
            printf("수정할 항목을 선택하세요:\n");
            printf("1. 학번\n2. 이름\n3. 학과\n4. 이메일\n5. 전화번호\n항목 선택: ");
            scanf("%d", &edit_choice);

            printf("새 값을 입력하세요: ");
            switch (edit_choice) {
            case 1:
                scanf("%d", &students[i].id);
                break;
            case 2:
                scanf("%s", students[i].name);
                break;
            case 3:
                scanf("%s", students[i].department);
                break;
            case 4:
                scanf("%s", students[i].email);
                break;
            case 5:
                scanf("%s", students[i].phone);
                break;
            default:
                printf("잘못된 선택입니다.\n");
                return;
            }
            printf("학생 정보가 수정되었습니다.\n");
            break;
        }
    }

    if (!found) {
        printf("해당 학번의 학생 정보를 찾을 수 없습니다.\n");
    } else {
        save_students(students, count);
    }
}

// 학생 정보를 삭제하는 함수
void delete_student() {
    int delete_id;
    printf("삭제할 학번 입력: ");
    scanf("%d", &delete_id);

    FILE *file = fopen(FILENAME, "r");
    FILE *temp_file = fopen(TEMP_FILENAME, "w");

    if (file == NULL || temp_file == NULL) {
        perror("파일 열기 오류");
        if (file) fclose(file);
        if (temp_file) fclose(temp_file);
        return;
    }

    char line[200];
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        Student student;
        sscanf(line, "%d,%[^,],%[^,],%[^,],%s", &student.id, student.name, student.department, student.email, student.phone);

        if (student.id != delete_id) {
            fprintf(temp_file, "%d,%s,%s,%s,%s\n", student.id, student.name, student.department, student.email, student.phone);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(temp_file);

    if (found) {
        remove(FILENAME);
        rename(TEMP_FILENAME, FILENAME);
        printf("학번 %d의 정보가 삭제되었습니다.\n", delete_id);
    } else {
        printf("학번 %d의 정보를 찾을 수 없습니다.\n", delete_id);
        remove(TEMP_FILENAME);
    }
}

int main() {
    int choice;

    while (1) {
        printf("\n1. 추가\n2. 검색\n3. 내보내기\n4. 수정\n5. 삭제\n6. 종료\n메뉴 선택: ");

        if (scanf("%d", &choice) != 1) {
            printf("잘못된 입력입니다. 다시 선택해 주세요.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
        case 1:
            add_student();
            break;
        case 2:
            search_student();
            break;
        case 3:
            export_students();
            break;
        case 4:
            edit_student();
            break;
        case 5:
            delete_student();
            break;
        case 6:
            printf("프로그램을 종료합니다.\n");
            exit(0);
        default:
            printf("잘못된 입력입니다. 다시 선택해 주세요.\n");
        }
    }

    return 0;
}