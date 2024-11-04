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

    // 사용자로부터 새로운 학생 정보 입력받기
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

    // 새 학생 정보를 배열에 추가
    students[count++] = student;

    // 학번을 기준으로 오름차순 정렬
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (students[i].id > students[j].id) {
                Student temp = students[i];
                students[i] = students[j];
                students[j] = temp;
            }
        }
    }

    // 정렬된 학생 정보를 파일에 저장
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

    // 검색 옵션 선택
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
        // 학번으로 검색하는 경우
        printf("검색할 학번 입력: ");
        scanf("%d", &search_id);
    } else {
        // 학과로 검색하는 경우
        printf("검색할 학과 입력: ");
        scanf("%s", search_term);
    }

    char line[200];
    int found = 0;

    // 파일을 한 줄씩 읽어가며 검색 조건에 맞는지 확인
    while (fgets(line, sizeof(line), file)) {
        Student student;
        sscanf(line, "%d,%[^,],%[^,],%[^,],%s", &student.id, student.name, student.department, student.email, student.phone);

        // 검색 조건에 따라 학번 또는 학과로 비교
        if ((search_type == 1 && student.id == search_id) ||  // 학번 검색
            (search_type == 2 && strcmp(student.department, search_term) == 0)) {  // 학과 검색
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

void export_students() {
    // 원본 파일을 읽기 모드 ("r")
    FILE *file = fopen(FILENAME, "r");
    char search_term[20];  // 검색어를 저장할 배열
    int export_type;       // 내보내기 방법을 저장할 변수

    // 혹여나 오류 처리
    if (file == NULL) {
        perror("파일 열기 오류");  // 오류 메시지를 출력합니다.
        return;  // 함수 종료
    }

    // 내보내기 옵션을 선택하는 곳
    printf("1. 전체 내보내기\n2. 특정 학번 내보내기\n3. 특정 학과 내보내기\n내보내기 방법 선택: ");
    scanf("%d", &export_type);

    // 특정 학번 또는 특정 학과를 선택한 경우 scanf로 입력 받기
    if (export_type == 2 || export_type == 3) {
        printf("검색어 입력: ");
        scanf("%s", search_term);
    }

    char line[200];        // 원본 파일에서 한 줄씩 읽어올 배열 (나중에 학생 수가 더 많으면 늘려야 함)
    char filename[50];     // 생성할 파일 이름을 저장할 배열
    FILE *output_file = NULL;  // 내보내기할 파일의 포인터

    // 내보내기 방식에 따라 파일 이름과 모드 설정
    if (export_type == 1) {
        strcpy(filename, "전체학생정보.txt");  // 전체 내보내기일 때의 파일 이름
        output_file = fopen(filename, "w");    // 쓰기 모드("w")로 새 파일을 열기
    } else if (export_type == 2) {
        sprintf(filename, "학번_%s_학생정보.txt", search_term);  // 학번 기준으로 파일 이름 설정
        output_file = fopen(filename, "w");
    } else if (export_type == 3) {
        sprintf(filename, "학과_%s_학생정보.txt", search_term);  // 학과 기준으로 파일 이름 설정
        output_file = fopen(filename, "w");
    }

    // 내보내기할 파일이 제대로 열리지 않았을 경우 혹시나 하는 오류처리
    if (output_file == NULL) {
        perror("파일 작성 오류");  // 오류 메시지를 출력합니다.
        fclose(file);  // 원본 파일 닫기
        return;        // 함수 종료
    }

    int found = 0;  // 검색 결과가 있는지 여부를 저장할 변수

    // 원본 파일을 한 줄씩 읽어가며 조건에 맞는 데이터 필터링
    while (fgets(line, sizeof(line), file)) {
        Student student;
        // 읽어온 데이터를 쉼표(,) 기준으로 분리하여 Student 구조체에 저장
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%s", student.id, student.name, student.department, student.email, student.phone);

        // 내보내기 조건을 확인하여 조건에 맞는 데이터만 파일에 기록
        if (export_type == 1 ||  // 전체 내보내기
            (export_type == 2 && strcmp(student.id, search_term) == 0) ||  // 학번이 검색어와 일치할 때
            (export_type == 3 && strcmp(student.department, search_term) == 0)) {  // 학과가 검색어와 일치할 때
            fprintf(output_file, "학번: %s, 이름: %s, 학과: %s, 이메일: %s, 전화번호: %s\n",
                    student.id, student.name, student.department, student.email, student.phone);
            found = 1;  // 검색 결과가 있음을 표시
        }
    }

    // 검색 결과가 없는 경우 사용자에게 알림
    if (!found && (export_type == 2 || export_type == 3)) {
        printf("내보낼 정보가 없습니다.\n");
    } else {
        printf("%s에 내보내기 완료되었습니다.\n", filename);
    }

    // 파일 포인터 닫기
    fclose(file);          // 원본 파일 닫기
    fclose(output_file);    // 내보내기 파일 닫기
}

// 학생 정보 수정 함수
void edit_student() {
    int edit_id;
    printf("수정할 학번 입력: ");
    scanf("%d", &edit_id);  // 수정할 학번을 int로 입력

    Student students[100];
    int count = load_students(students);
    int found = 0;

    for (int i = 0; i < count; i++) {
        if (students[i].id == edit_id) {  // 학번을 int로 비교
            found = 1;
            int edit_choice;
            printf("수정할 항목을 선택하세요:\n");
            printf("1. 학번\n2. 이름\n3. 학과\n4. 이메일\n5. 전화번호\n항목 선택: ");
            scanf("%d", &edit_choice);

            printf("새 값을 입력하세요: ");
            switch (edit_choice) {
            case 1:
                // 학번 수정 - 새 학번을 int로 입력받음
                    scanf("%d", &students[i].id);
                break;
            case 2:
                // 이름 수정
                    scanf("%s", students[i].name);
                break;
            case 3:
                // 학과 수정
                    scanf("%s", students[i].department);
                break;
            case 4:
                // 이메일 수정
                    scanf("%s", students[i].email);
                break;
            case 5:
                // 전화번호 수정
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
    scanf("%d", &delete_id);  // 삭제할 학번을 int로 입력받음

    // 원본 파일을 읽기 전용 모드로 열고, 임시 파일을 쓰기 모드로 엽니다.
    // 임시파일을 사용하는 이유는 C에서는 파일에서 특정 데이터를 직접 삭제할 수 없습니다.
    // 따라서 원본 파일의 데이터를 모두 읽고, 삭제할 데이터만 제외한 나머지 데이터를 임시 파일에 기록합니다.
    // 이후 원본 파일을 삭제하고 임시 파일을 원본 파일 이름으로 변경하여 삭제된 내용이 반영된 새 파일을 만듭니다.

    FILE *file = fopen(FILENAME, "r");
    FILE *temp_file = fopen(TEMP_FILENAME, "w");

    // 파일 열기 오류 처리
    if (file == NULL || temp_file == NULL) {
        perror("파일 열기 오류");
        if (file) fclose(file);
        if (temp_file) fclose(temp_file);
        return;
    }

    char line[200];
    int found = 0;

    // 파일을 한 줄씩 읽어가며 삭제할 학번과 비교
    while (fgets(line, sizeof(line), file)) {
        Student student;
        sscanf(line, "%d,%[^,],%[^,],%[^,],%s", &student.id, student.name, student.department, student.email, student.phone);

        // 학번이 삭제할 ID와 일치하지 않으면 임시 파일에 기록
        if (student.id != delete_id) {
            fprintf(temp_file, "%d,%s,%s,%s,%s\n", student.id, student.name, student.department, student.email, student.phone);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(temp_file);

    if (found) {
        // 원본 파일 삭제 후 임시 파일을 원본 파일 이름으로 변경
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