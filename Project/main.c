//202410288 유지헌 개인 프로젝트 과제

#include <stdio.h>   // 표준 입출력 라이브러리
#include <stdlib.h>  // 일반 유틸리티 함수 라이브러리
#include <string.h>  // 문자열 처리 라이브러리

// 파일 이름 상수 정의
#define FILENAME "학생정보파일.txt"     // 학생 정보 파일 이름
#define TEMP_FILENAME "임시파일.txt"    // 임시 파일 이름

// 학생 정보를 저장할 구조체 정의
typedef struct {
    int id;              // 학번
    char name[20];       // 이름
    char department[20]; // 학과
    char email[30];      // 이메일
    char phone[15];      // 전화번호
} Student;

// 학생 정보 불러오기 함수
int load_students(Student *students) {
    FILE *file = fopen(FILENAME, "r"); // 파일을 읽기 전용 모드로 염
    int count = 0;                     // 학생 수를 카운트할 변수

    if (file == NULL) return 0;        // 파일이 없는 경우 0 반환

    char line[400];                    // 파일의 각 줄을 저장할 배열
    // 파일의 각 줄을 읽어들여 학생 구조체에 저장
    while (fgets(line, sizeof(line), file)) {
        // 쉼표로 구분된 데이터를 구조체 필드에 채워 넣음
        sscanf(line, "%d,%[^,],%[^,],%[^,],%s", &students[count].id, students[count].name,
               students[count].department, students[count].email, students[count].phone);
        count++;                       // 학생 수 증가
    }
    fclose(file);                      // 파일 닫기
    return count;                      // 학생 수 반환
}

// 학생 정보 저장 함수
void save_students(Student *students, int count) {
    FILE *file = fopen(FILENAME, "w"); // 파일을 쓰기 모드로 열기
    for (int i = 0; i < count; i++) {  // 모든 학생 정보 파일에 기록
        fprintf(file, "%d,%s,%s,%s,%s\n", students[i].id, students[i].name, students[i].department,
                students[i].email, students[i].phone);
    }
    fclose(file);                      // 파일 닫기
}

// 학생 정보 추가 함수
void add_student() {
    Student student;                   // 새로운 학생 정보를 저장할 변수
    Student students[400];             // 최대 400명의 학생을 저장할 배열 나중에 늘려야 함

    int count = load_students(students); // 기존 학생 정보를 불러와 배열에 저장

    // 사용자로부터 새로운 학생 정보 입력받기
    printf("학번 입력: ");
    scanf("%d", &student.id);          // 학번을 정수로 입력
    printf("이름 입력: ");
    scanf("%s", student.name);         // 이름 입력
    printf("학과 입력: ");
    scanf("%s", student.department);   // 학과 입력
    printf("이메일 입력: ");
    scanf("%s", student.email);        // 이메일 입력
    printf("전화번호 입력( - 제외): ");
    scanf("%s", student.phone);        // 전화번호 입력

    students[count++] = student;       // 새 학생 정보를 배열에 추가

    // 학번을 기준으로 오름차순 정렬
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (students[i].id > students[j].id) { // 학번이 작은 순서대로 정렬
                Student temp = students[i];        // 임시 변수를 사용해 위치 교환
                students[i] = students[j];
                students[j] = temp;
            }
        }
    }

    save_students(students, count);    // 정렬된 학생 정보를 파일에 저장
    printf("학생 정보가 추가되었습니다. (버블정렬 하는 중..)\n");
}

// 학생 정보 검색 함수
void search_student() {
    FILE *file = fopen(FILENAME, "r"); // 파일을 읽기 모드로 열기
    int search_type;                   // 검색 타입 변수

    if (file == NULL) {                // 파일이 열리지 않으면 오류 메시지 출력 후 종료
        perror("파일 열기 오류");         // 빨간 글씨 오류
        return;
    }

    // 검색 옵션 선택
    while (1) {
        printf("1. 학번으로 검색\n2. 학과로 검색\n검색 방법 선택: ");
        if (scanf("%d", &search_type) != 1) {      // 정수로 입력을 받음
            printf("잘못된 입력입니다. 1 또는 2를 선택해 주세요.\n");
            while (getchar() != '\n');             // 잘못된 입력이 있을 경우 버퍼 비우기
            continue;
        }
        if (search_type != 1 && search_type != 2) { // 입력이 1 또는 2가 아닐 경우 오류 메시지
            printf("잘못된 입력입니다. 1 또는 2를 선택해 주세요.\n");
            continue;
        }
        break;                                     // 올바른 입력일 경우 반복문 탈출
    }

    char search_term[20];           // 학과 검색어를 저장할 변수
    int search_id = 0;              // 학번 검색어를 저장할 변수

    if (search_type == 1) {         // 학번으로 검색할 경우
        printf("검색할 학번 입력: ");
        scanf("%d", &search_id);    // 학번 입력받기
    } else {                        // 학과로 검색할 경우
        printf("검색할 학과 입력: ");
        scanf("%s", search_term);   // 학과 입력받기
    }

    char line[300];                 // 파일의 각 줄을 저장할 배열 총 학생 수
    int found = 0;                  // 검색 결과가 있는지 여부를 저장할 변수

    // 파일을 한 줄씩 읽어가며 검색 조건에 맞는지 확인
    while (fgets(line, sizeof(line), file)) {
        Student student;            // 각 학생 정보를 임시로 저장할 구조체
        sscanf(line, "%d,%[^,],%[^,],%[^,],%s", &student.id, student.name, student.department, student.email, student.phone);

        // 검색 조건에 따라 학번 또는 학과로 비교
        if ((search_type == 1 && student.id == search_id) ||  // 학번 검색
            (search_type == 2 && strcmp(student.department, search_term) == 0)) {  // 학과 검색
            printf("학번: %d, 이름: %s, 학과: %s, 이메일: %s, 전화번호: %s\n",
                   student.id, student.name, student.department, student.email, student.phone);
            found = 1;              // 검색 결과가 있음을 표시
        }
    }

    if (!found) {                   // 검색 결과가 없을 경우
        printf("검색 결과가 없습니다.\n");
    }

    fclose(file);                   // 파일 닫기
}

// 학생 정보 내보내기 함수
void export_students() {
    FILE *file = fopen(FILENAME, "r"); // 원본 파일을 읽기 모드로 염
    char search_term[20];              // 검색어를 저장할 배열
    int export_type;                   // 내보내기 방법을 저장할 변수

    if (file == NULL) {                // 파일 열기 오류 처리
        perror("파일 열기 오류");         // 빨간 글씨 오류
        return;
    }

    // 내보내기 옵션 선택
    printf("1. 전체 내보내기\n2. 특정 학번 내보내기\n3. 특정 학과 내보내기\n내보내기 방법 선택: ");
    scanf("%d", &export_type);

    // 특정 학번 또는 특정 학과를 선택한 경우 검색어 입력 받기
    if (export_type == 2 || export_type == 3) {
        printf("검색어 입력: ");
        scanf("%s", search_term);
    }

    char line[400];                   // 원본 파일에서 한 줄씩 읽어올 배열 총 학생수와 같음
    char filename[50];                // 생성할 파일 이름을 저장할 배열
    FILE *output_file = NULL;         // 내보내기할 파일의 포인터

    // 내보내기 방식에 따라 파일 이름과 모드 설정
    if (export_type == 1) {
        strcpy(filename, "전체학생정보.txt");       // 전체 내보내기일 때의 파일 이름 설정
        output_file = fopen(filename, "w");         // 쓰기 모드로 새 파일 열기
    } else if (export_type == 2) {
        sprintf(filename, "학번_%s_학생정보.txt", search_term); // 학번 기준으로 파일 이름 설정
        output_file = fopen(filename, "w");
    } else if (export_type == 3) {
        sprintf(filename, "학과_%s_학생정보.txt", search_term); // 학과 기준으로 파일 이름 설정
        output_file = fopen(filename, "w");
    }

    if (output_file == NULL) {        // 내보내기 파일 열기 오류 처리
        perror("파일 작성 오류");        // 빨간 글씨 오류
        fclose(file);                 // 원본 파일 닫기
        return;
    }

    int found = 0;                    // 검색 결과가 있는지 여부를 저장할 변수

    // 원본 파일을 한 줄씩 읽어가며 조건에 맞는 데이터 필터링
    while (fgets(line, sizeof(line), file)) {
        Student student;              // 학생 정보를 저장할 구조체
        sscanf(line, "%d,%[^,],%[^,],%[^,],%s", &student.id, student.name, student.department, student.email, student.phone);

        char id_str[20];              // 학번을 문자열로 변환하여 저장할 배열
        sprintf(id_str, "%d", student.id); // 학번을 문자열로 변환

        // 내보내기 조건을 확인하여 조건에 맞는 데이터만 파일에 기록
        if (export_type == 1 ||                                // 전체 내보내기
            (export_type == 2 && strcmp(id_str, search_term) == 0) || // 학번이 검색어와 일치할 때
            (export_type == 3 && strcmp(student.department, search_term) == 0)) {  // 학과가 검색어와 일치할 때
            fprintf(output_file, "학번: %d, 이름: %s, 학과: %s, 이메일: %s, 전화번호: %s\n",
                    student.id, student.name, student.department, student.email, student.phone);
            found = 1;                // 검색 결과가 있음을 표시
        }
    }

    // 검색 결과가 없는 경우 사용자에게 알림
    if (!found && (export_type == 2 || export_type == 3)) {
        printf("내보낼 학생 정보가 없습니다.\n");
    } else {
        printf("%s의 내보내기가 완료되었습니다.\n", filename); // 내보내기 완료 메시지 출력
    }

    fclose(file);                     // 원본 파일 닫기
    fclose(output_file);               // 내보내기 파일 닫기
}

// 학생 정보 수정 함수
void edit_student() {
    int edit_id;                          // 수정할 학번을 저장할 변수
    printf("수정할 학번 입력: ");
    scanf("%d", &edit_id);                // 사용자로부터 수정할 학번 입력받기

    Student students[400];                // 학생 정보를 저장할 배열 추후에 추가 가능
    int count = load_students(students);  // 파일에서 학생 정보를 불러와 배열에 저장
    int found = 0;                        // 수정할 학생을 찾았는지 여부를 저장할 변수

    // 학생 배열을 순회하며 수정할 학생을 찾음
    for (int i = 0; i < count; i++) {
        if (students[i].id == edit_id) {  // 학번이 일치하는 학생을 찾으면
            found = 1;                    // 학생을 찾았음을 표시
            int edit_choice;              // 수정할 항목 선택을 저장할 변수

            // 수정할 항목을 사용자에게 선택받음
            printf("수정할 항목을 선택하세요:\n");
            printf("1. 학번\n2. 이름\n3. 학과\n4. 이메일\n5. 전화번호\n항목 선택: ");
            scanf("%d", &edit_choice);    // 항목 선택 입력받기

            printf("수정할 값을 입력하세요: "); // 새 값 입력 요청

            switch (edit_choice) {
            case 1:
                scanf("%d", &students[i].id);        // 학번 수정
                break;
            case 2:
                scanf("%s", students[i].name);       // 이름 수정
                break;
            case 3:
                scanf("%s", students[i].department); // 학과 수정
                break;
            case 4:
                scanf("%s", students[i].email);     // 이메일 수정
                break;
            case 5:
                scanf("%s", students[i].phone);     // 전화번호 수정
                break;
            default:
                printf("잘못된 선택입니다.\n");     // 잘못된 선택 시 오류 메시지
                return;                         // 함수 종료
            }
            printf("학생 정보가 수정되었습니다.\n");    // 수정 완료 메시지 출력
            break;                               // 수정 완료 후 반복문 종료
        }
    }

    if (!found) {               // 수정할 학생을 찾지 못한 경우
        printf("해당 학번의 학생 정보를 찾을 수 없습니다.\n");
    } else {
        save_students(students, count); // 수정된 정보를 파일에 저장
    }
}

// 학생 정보를 삭제하는 함수
void delete_student() {
    int delete_id;                          // 삭제할 학번을 저장할 변수
    printf("삭제할 학번 입력: ");
    scanf("%d", &delete_id);                // 사용자로부터 삭제할 학번 입력받기

    FILE *file = fopen(FILENAME, "r");      // 원본 파일을 읽기 전용 모드로 열기
    FILE *temp_file = fopen(TEMP_FILENAME, "w"); // 임시 파일을 쓰기 모드로 열기

    // 파일 열기 오류 처리
    if (file == NULL || temp_file == NULL) {
        perror("파일 열기 오류");            // 오류 메시지 출력
        if (file) fclose(file);              // 파일이 열려 있으면 닫기
        if (temp_file) fclose(temp_file);    // 임시 파일이 열려 있으면 닫기
        return;                              // 함수 종료
    }

    char line[400];                // 파일에서 한 줄씩 읽어올 배열
    int found = 0;                 // 삭제할 학번을 찾았는지 여부를 저장할 변수

    // 파일을 한 줄씩 읽어가며 삭제할 학번과 비교
    while (fgets(line, sizeof(line), file)) {
        Student student;           // 학생 정보를 저장할 구조체
        sscanf(line, "%d,%[^,],%[^,],%[^,],%s", &student.id, student.name, student.department, student.email, student.phone);

        // 학번이 삭제할 ID와 일치하지 않으면 임시 파일에 기록
        if (student.id != delete_id) {
            fprintf(temp_file, "%d,%s,%s,%s,%s\n", student.id, student.name, student.department, student.email, student.phone);
        } else {
            found = 1;              // 삭제할 학생 정보를 찾았음을 표시
        }
    }

    fclose(file);                   // 원본 파일 닫기
    fclose(temp_file);               // 임시 파일 닫기

    if (found) {                     // 삭제할 학번을 찾은 경우
        remove(FILENAME);            // 원본 파일 삭제
        rename(TEMP_FILENAME, FILENAME); // 임시 파일을 원본 파일 이름으로 변경
        printf("학번 %d의 정보가 삭제되었습니다.\n", delete_id); // 삭제 완료 메시지
    } else {                         // 삭제할 학번을 찾지 못한 경우
        printf("학번 %d의 정보를 찾을 수 없습니다.\n", delete_id); // 오류 메시지
        remove(TEMP_FILENAME);       // 임시 파일 삭제
    }
}

// 프로그램의 메인 함수
int main() {
    int choice;                      // 사용자 메뉴 선택을 저장할 변수

    while (1) {                      // 무한 반복하여 메뉴 표시
        // 메뉴 옵션을 출력하여 사용자에게 선택 요청
        printf("\n1. 추가\n2. 검색\n3. 내보내기\n4. 수정\n5. 삭제\n6. 종료\n메뉴 선택: ");

        // 사용자가 정수 값을 입력하지 않을 경우 오류 처리
        if (scanf("%d", &choice) != 1) {
            printf("잘못된 입력입니다. 다시 선택해 주세요.\n");
            while (getchar() != '\n'); // 입력 버퍼를 비워 잘못된 입력 제거
            continue;
        }

        // 선택한 메뉴에 따라 해당 기능 수행
        switch (choice) {
        case 1:
            add_student();           // 학생 정보 추가 함수 호출
            break;
        case 2:
            search_student();        // 학생 정보 검색 함수 호출
            break;
        case 3:
            export_students();       // 학생 정보 내보내기 함수 호출
            break;
        case 4:
            edit_student();          // 학생 정보 수정 함수 호출
            break;
        case 5:
            delete_student();        // 학생 정보 삭제 함수 호출
            break;
        case 6:
            printf("프로그램을 종료합니다.\n"); // 종료 메시지 출력
            exit(0);                 // 프로그램 종료
        default:
            printf("잘못된 입력입니다. 다시 선택해 주세요.\n"); // 잘못된 선택 시 오류 메시지
        }
    }

    return 0;                        // main 함수 종료
}