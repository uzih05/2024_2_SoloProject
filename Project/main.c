
//202410288 유지헌 개인 프로젝트 과제

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "학생정보파일.txt"
#define TEMP_FILENAME "임시파일.txt"

// 학생 정보를 저장할 구조체 정의
typedef struct {
    char id[10];
    char name[20];
    char department[20];
    char email[30];
    char phone[15];
} Student;

// 줄바꿈 문자 제거 함수
void remove_newline(char *str) {
    str[strcspn(str, "\n")] = '\0';
}

// 학생 정보를 파일에 추가하는 함수
void add_student() {
    Student student;
    // 파일 포인터 `file`을 선언
    // "a" 모드로 파일이 존재하면 추가, 존재하지 않다면 새로 만듬.
    FILE *file = fopen(FILENAME, "a");

    // 파일 여는데에 실패하진 않겠지만, 혹여나 만들어둔 오류 체크기입니다.
    if (file == NULL) {
        perror("파일 열기 실패."); // 오류 메시지 출력
        return; // 함수 종료
    }

    // 사용자로부터 학생 정보를 입력받습니다.
    printf("학번 입력: ");
    scanf("%s", student.id);
    printf("이름 입력: ");
    scanf("%s", student.name);
    printf("학과 입력: ");
    scanf("%s", student.department);
    printf("이메일 입력: ");
    scanf("%s", student.email);
    printf("전화번호 입력( - 제외): ");
    scanf("%s", student.phone);

    // `fprintf` 함수로 파일에 데이터를 기록합니다.
    // `file` 포인터는 파일에 접근하는 스트림이며, `fprintf`는 해당 파일에 포맷에 맞게 데이터를 씁니다.
    fprintf(file, "%s,%s,%s,%s,%s\n", student.id, student.name, student.department, student.email, student.phone);

    // 파일을 닫습니다. (필수)
    fclose(file);

    printf("학생 정보가 추가되었습니다.\n");
}

// 학생정보파일에서 학생 정보를 검색하는 함수
void search_student() {
    // 파일을 "r" 로 엽니다.
    FILE *file = fopen(FILENAME, "r");
    char search_term[20];
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

    printf("검색어 입력: ");
    scanf("%s", search_term);

    char line[200];
    int found = 0;

    // `fgets` 함수를 사용해 한 줄씩 읽습니다.
    // `fgets`는 `file` 포인터가 가리키는 파일에서 문자열을 읽어와 `line` 배열에 저장합니다.
    while (fgets(line, sizeof(line), file)) {
        Student student;
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%s", student.id, student.name, student.department, student.email, student.phone);

        if ((search_type == 1 && strcmp(student.id, search_term) == 0) ||
            (search_type == 2 && strcmp(student.department, search_term) == 0)) {
            printf("학번: %s, 이름: %s, 학과: %s, 이메일: %s, 전화번호: %s\n", student.id, student.name, student.department, student.email, student.phone);
            found = 1;
        }
    }

    if (!found) {
        printf("검색 결과가 없습니다.\n");
    }

    // 읽기 작업이 끝난 후 파일을 닫습니다.
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


// 학생 정보를 삭제하는 함수
void delete_student() {
    char delete_id[10];
    printf("삭제할 학번 입력: ");
    scanf("%s", delete_id);

    // 원본 파일을 읽기 전용 모드로 열고, 임시 파일을 쓰기 모드로 엽니다.
    // 임시파일을 사용하는 이유는 C에서는 파일에서 특정 데이터를 직접 삭제할 수 없습니다.
    // 따라서 원본 파일의 데이터를 모두 읽고, 삭제할 데이터만 제외한 나머지 데이터를 임시 파일에 기록합니다.
    // 이후 원본 파일을 삭제하고 임시 파일을 원본 파일 이름으로 변경하여 삭제된 내용이 반영된 새 파일을 만듭니다.

    FILE *file = fopen(FILENAME, "r");
    FILE *temp_file = fopen(TEMP_FILENAME, "w");

    // 혹시나 하는 오류로 인한 에러 체크기
    if (file == NULL || temp_file == NULL) {
        perror("파일 열기 오류");
        if (file) fclose(file);
        if (temp_file) fclose(temp_file);
        return;
    }

    char line[200]; // 원본 파일에서 한 줄씩 읽어올 배열 (나중에 학생 수가 더 많으면 늘려야 함)
    int found = 0;

    // 이 코드는 원본 파일에서 한 줄씩 학생 정보를 읽어와, 삭제할 학번과 비교하여 삭제 대상이 아닌 데이터만 임시 파일에 기록합니다.
    // 학번이 삭제할 ID와 일치하지 않는 경우에만 임시 파일에 저장합니다.
    // 학번이 삭제할 ID와 일치하는 경우에는 found 변수를 1로 설정하여, 삭제할 데이터가 존재했음을 표시합니다.
    // 마지막에 임시 파일을 원본 파일로 대체하여, 삭제된 데이터를 반영한 파일을 생성합니다.
    while (fgets(line, sizeof(line), file)) {
        Student student;
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%s", student.id, student.name, student.department, student.email, student.phone);

        // 학번이 일치하지 않으면 임시 파일에 기록
        if (strcmp(student.id, delete_id) != 0) {
            fprintf(temp_file, "%s,%s,%s,%s,%s\n", student.id, student.name, student.department, student.email, student.phone);
        } else {
            found = 1;
        }
    }

    // 파일은 꼭 닫아주기.
    fclose(file);
    fclose(temp_file);

    if (found) {
        // 원본 파일 삭제 후 임시 파일로 바꿔치기
        remove(FILENAME);
        rename(TEMP_FILENAME, FILENAME);
        printf("학번 %s의 정보가 삭제되었습니다.\n", delete_id);
    } else {
        printf("학번 %s의 정보를 찾을 수 없습니다.\n", delete_id);
        remove(TEMP_FILENAME);
    }
}

int main() {
    int choice;

    while (1) {
        printf("\n1. 추가\n2. 검색\n3. 내보내기\n4. 삭제\n5. 종료\n메뉴 선택: ");

        if (scanf("%d", &choice) != 1) {
            printf("잘못된 입력입니다. 다시 선택해 주세요.\n");
            while (getchar() != '\n');
            continue;
        }

        // 교과서에서 배운 switch 적극 활용
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
                delete_student();
                break;
            case 5:
                printf("프로그램을 종료합니다.\n");
                exit(0);
            default:
                printf("잘못된 입력입니다. 다시 선택해 주세요.\n");
        }
    }

    return 0;
}
