#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "학생정보파일.txt"
#define TEMP_FILENAME "임시파일.txt"

// 학생 정보를 저장할 구조체 정의
// 학번은 20자리 문자열, 이름 20자리, 학과는 15자리로 제한
typedef struct {
    int id;                    // 학번 (정수형으로 유지)
    char name[20];             // 학생 이름 (최대 20자)
    char department[15];       // 학과 (최대 15자)
    char email[30];            // 이메일 주소
    char phone[15];            // 전화번호
} Student;

// 문자열의 끝에서 줄바꿈 문자를 제거하는 함수
void remove_newline(char *str) {
    str[strcspn(str, "\n")] = '\0'; // 개행 문자('\n') 위치에 '\0'을 삽입해 문자열 종료
}

// 학생 정보를 파일에 추가하는 함수
void add_student() {
    Student student;
    Student students[100]; // 최대 100명의 학생을 처리할 수 있는 배열
    int count = 0;

    // 기존 파일에서 학생 정보를 모두 읽어와 배열에 저장
    FILE *file = fopen(FILENAME, "r");
    if (file != NULL) { // 기존 파일이 있으면 정보를 읽어옴
        char line[200];
        while (fgets(line, sizeof(line), file)) {
            sscanf(line, "%d,%[^,],%[^,],%[^,],%s", &students[count].id, students[count].name,
                   students[count].department, students[count].email, students[count].phone);
            count++;
        }
        fclose(file);
    }

    // 새 학생 정보 입력
    printf("학번 입력: ");
    scanf("%d", &student.id);
    printf("이름 입력 (최대 20자): ");
    scanf("%s", student.name);
    printf("학과 입력 (최대 15자): ");
    scanf("%s", student.department);
    printf("이메일 입력: ");
    scanf("%s", student.email);
    printf("전화번호 입력 ( - 제외): ");
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

    // 정렬된 학생 정보를 파일에 다시 저장
    file = fopen(FILENAME, "w");
    if (file == NULL) {
        perror("파일 열기 실패.");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d,%s,%s,%s,%s\n", students[i].id, students[i].name, students[i].department,
                students[i].email, students[i].phone);
    }
    fclose(file);

    printf("학생 정보가 추가되었습니다.\n");
}

// 학생 정보를 파일에서 검색하는 함수
void search_student() {
    FILE *file = fopen(FILENAME, "r"); // 파일을 읽기 전용 모드로 열기
    int search_id;                     // 학번 검색용 변수
    char search_term[20];              // 학과 검색용 문자열
    int search_type;                   // 검색 방법 선택 (1: 학번, 2: 학과)

    if (file == NULL) { // 파일을 열 수 없을 때
        perror("파일 열기 오류"); // 파일 열기 오류 출력
        return;
    }

    // 검색 방법을 사용자로부터 선택받음
    while (1) {
        printf("1. 학번으로 검색\n2. 학과로 검색\n검색 방법 선택: ");
        if (scanf("%d", &search_type) != 1) { // 올바른 정수 입력인지 확인
            printf("잘못된 입력입니다. 1 또는 2를 선택해 주세요.\n");
            while (getchar() != '\n'); // 잘못된 입력 처리 (버퍼 비우기)
            continue;
        }
        if (search_type != 1 && search_type != 2) { // 잘못된 선택 값일 경우
            printf("잘못된 입력입니다. 1 또는 2를 선택해 주세요.\n");
            continue;
        }
        break; // 유효한 선택 시 반복 종료
    }

    // 검색 조건에 따라 검색할 데이터 입력받음
    if (search_type == 1) { // 학번으로 검색
        printf("학번 입력: ");
        scanf("%d", &search_id); // 학번을 정수로 입력
    } else { // 학과로 검색
        printf("학과 입력: ");
        scanf("%s", search_term); // 학과 이름 입력
    }

    char line[200]; // 파일에서 한 줄씩 읽어올 배열 (나중에 학생 수가 많으면 늘려야 함!)
    int found = 0;  // 검색 결과 유무 표시

    // 파일에서 한 줄씩 읽어와 검색 조건과 일치하는지 확인
    while (fgets(line, sizeof(line), file)) {
        Student student;
        // 파일에서 읽어온 데이터를 쉼표로 구분하여 Student 구조체에 저장
        sscanf(line, "%d,%[^,],%[^,],%[^,],%s", &student.id, student.name, student.department, student.email, student.phone);

        // 검색 조건이 일치할 경우 화면에 학생 정보 출력
        if ((search_type == 1 && student.id == search_id) ||  // 학번이 일치
            (search_type == 2 && strcmp(student.department, search_term) == 0)) { // 학과가 일치
            printf("학번: %d, 이름: %s, 학과: %s, 이메일: %s, 전화번호: %s\n", student.id, student.name, student.department, student.email, student.phone);
            found = 1; // 결과가 있음 표시
        }
    }

    if (!found) { // 검색 결과가 없을 경우
        printf("검색 결과가 없습니다.\n");
    }

    fclose(file); // 파일 닫기
}

// 학생 정보를 수정하는 함수
void update_student() {
    int search_id, choice; // 학번과 수정할 항목 선택 변수
    char new_value[30];    // 수정할 새 값
    int found = 0;         // 해당 학번이 존재하는지 여부

    printf("수정할 학생의 학번 입력: ");
    scanf("%d", &search_id); // 수정할 학생의 학번 입력

    Student students[100]; // 최대 100명의 학생 정보를 담을 배열 추후에 늘려야 함
    int count = 0;         // 학생 수 카운트

    FILE *file = fopen(FILENAME, "r"); // 원본 파일 읽기 모드로 열기
    if (file == NULL) { // 파일 열기 오류 처리
        perror("파일 열기 오류");
        return;
    }

    // 파일에서 모든 학생 정보를 읽어와 배열에 저장
    char line[200];
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%d,%19[^,],%14[^,],%29[^,],%14s", &students[count].id, students[count].name,
               students[count].department, students[count].email, students[count].phone);
        count++;
    }
    fclose(file); // 파일 닫기

    // 배열에서 해당 학번을 가진 학생을 찾아 수정
    for (int i = 0; i < count; i++) {
        if (students[i].id == search_id) { // 학번이 일치하는 경우
            found = 1; // 학번이 존재함 표시
            printf("수정할 부분을 선택하세요:\n1. 이름\n2. 학과\n3. 이메일\n4. 전화번호\n선택: ");
            scanf("%d", &choice); // 수정할 필드 선택

            // 선택한 필드에 따라 값 변경
            printf("수정할 값을 입력하세요: ");
            scanf("%s", new_value);

            switch (choice) {
                case 1:
                    strncpy(students[i].name, new_value, sizeof(students[i].name) - 1);
                    students[i].name[sizeof(students[i].name) - 1] = '\0';
                    break;
                case 2:
                    strncpy(students[i].department, new_value, sizeof(students[i].department) - 1);
                    students[i].department[sizeof(students[i].department) - 1] = '\0';
                    break;
                case 3:
                    strncpy(students[i].email, new_value, sizeof(students[i].email) - 1);
                    students[i].email[sizeof(students[i].email) - 1] = '\0';
                    break;
                case 4:
                    strncpy(students[i].phone, new_value, sizeof(students[i].phone) - 1);
                    students[i].phone[sizeof(students[i].phone) - 1] = '\0';
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
        printf("학번 %d의 정보를 찾을 수 없습니다.\n", search_id);
        return;
    }

    // 수정된 전체 데이터를 파일에 다시 저장
    file = fopen(FILENAME, "w"); // 파일을 쓰기 모드로 열어 기존 내용을 덮어씀
    if (file == NULL) {
        perror("파일 열기 오류");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d,%-19s,%-14s,%-29s,%-14s\n", students[i].id, students[i].name,
                students[i].department, students[i].email, students[i].phone);
    }
    fclose(file); // 파일 닫기
}


// 학생 정보를 파일로 내보내는 함수
void export_students() {
    FILE *file = fopen(FILENAME, "r"); // "r" 모드로 파일 열기
    char search_term[20];              // 검색어 저장
    int export_type;                   // 내보내기 방법 선택 (1: 전체, 2: 학번별, 3: 학과별)

    if (file == NULL) {
        perror("파일 열기 오류"); // 파일 열기 실패 시 오류 출력
        return;
    }

    // 내보내기 옵션 선택
    printf("1. 전체 내보내기\n2. 특정 학번 내보내기\n3. 특정 학과 내보내기\n내보내기 방법 선택: ");
    scanf("%d", &export_type);

    // 학번별 또는 학과별 선택 시 검색어 입력
    if (export_type == 2 || export_type == 3) {
        printf("검색어 입력: ");
        scanf("%s", search_term);
    }

    char line[200];             // 파일에서 한 줄씩 읽어올 배열 (나중에 학생 수가 많으면 늘려야 함!)
    char filename[50];          // 내보낼 파일명
    FILE *output_file = NULL;   // 내보내기할 파일 포인터

    // 내보내기 방식에 따른 파일명 설정
    if (export_type == 1) {
        strcpy(filename, "전체학생정보.txt"); // 전체 내보내기
        output_file = fopen(filename, "w");
    } else if (export_type == 2) {
        sprintf(filename, "학번_%s_학생정보.txt", search_term); // 학번별 내보내기
        output_file = fopen(filename, "w");
    } else if (export_type == 3) {
        sprintf(filename, "학과_%s_학생정보.txt", search_term); // 학과별 내보내기
        output_file = fopen(filename, "w");
    }

    if (output_file == NULL) {
        perror("파일 작성 오류"); // 파일 작성 실패 시 오류 출력
        fclose(file);
        return;
    }

    int found = 0; // 내보낼 데이터 유무 확인

    // 파일에서 한 줄씩 읽어 조건에 맞는 데이터 내보내기
    while (fgets(line, sizeof(line), file)) {
        Student student;
        sscanf(line, "%d,%[^,],%[^,],%[^,],%s", &student.id, student.name, student.department, student.email, student.phone);

        // 선택된 조건에 따라 데이터를 파일에 기록
        if (export_type == 1 || // 전체 내보내기
            (export_type == 2 && student.id == atoi(search_term)) || // 학번 조건
            (export_type == 3 && strcmp(student.department, search_term) == 0)) { // 학과 조건
            fprintf(output_file, "학번: %d, 이름: %s, 학과: %s, 이메일: %s, 전화번호: %s\n",
                    student.id, student.name, student.department, student.email, student.phone);
            found = 1;
        }
    }

    // 내보낼 데이터가 없는 경우 메시지 출력
    if (!found && (export_type == 2 || export_type == 3)) {
        printf("내보낼 정보가 없습니다.\n");
    } else {
        printf("%s에 내보내기 완료되었습니다.\n", filename);
    }

    fclose(file);          // 원본 파일 닫기
    fclose(output_file);    // 내보내기 파일 닫기
}

// 학생 정보를 삭제하는 함수
void delete_student() {
    int delete_id;
    printf("삭제할 학번 입력: ");
    scanf("%d", &delete_id); // 삭제할 학번 입력

    FILE *file = fopen(FILENAME, "r");      // 원본 파일 읽기 모드로 열기
    FILE *temp_file = fopen(TEMP_FILENAME, "w"); // 임시 파일 쓰기 모드로 열기

    if (file == NULL || temp_file == NULL) { // 파일 열기 오류 처리
        perror("파일 열기 오류");
        if (file) fclose(file);
        if (temp_file) fclose(temp_file);
        return;
    }

    char line[200]; // 파일에서 한 줄씩 읽어올 배열 (나중에 학생 수가 많으면 늘려야 함!)
    int found = 0;  // 학번 존재 여부 확인

    // 파일에서 한 줄씩 읽어와 삭제할 학번을 제외한 데이터만 임시 파일에 기록
    while (fgets(line, sizeof(line), file)) {
        Student student;
        sscanf(line, "%d,%[^,],%[^,],%[^,],%s", &student.id, student.name, student.department, student.email, student.phone);

        if (student.id != delete_id) { // 삭제할 학번이 아닌 경우 임시 파일에 기록
            fprintf(temp_file, "%d,%s,%s,%s,%s\n", student.id, student.name, student.department, student.email, student.phone);
        } else {
            found = 1; // 학번이 존재함 표시
        }
    }

    fclose(file);           // 원본 파일 닫기
    fclose(temp_file);      // 임시 파일 닫기

    if (found) { // 삭제할 학번이 있는 경우
        remove(FILENAME);            // 원본 파일 삭제
        rename(TEMP_FILENAME, FILENAME); // 임시 파일을 원본 파일명으로 변경
        printf("학번 %d의 정보가 삭제되었습니다.\n", delete_id);
    } else { // 삭제할 학번이 없는 경우
        printf("학번 %d의 정보를 찾을 수 없습니다.\n", delete_id);
        remove(TEMP_FILENAME); // 임시 파일 삭제
    }
}

// 메인 함수: 메뉴 선택 및 프로그램 실행
int main() {

    int choice;

    while (1) {
        // 사용자에게 메뉴를 보여주고 선택을 요청
        printf("\n1. 추가\n2. 검색\n3. 내보내기\n4. 수정\n5. 삭제\n6. 종료\n메뉴 선택: ");

        if (scanf("%d", &choice) != 1) { // 정수가 아닌 입력이 들어올 경우
            printf("잘못된 입력입니다. 다시 선택해 주세요.\n");
            while (getchar() != '\n'); // 버퍼 비우기
            continue;
        }

        // 선택된 메뉴에 따라 함수 호출
        switch (choice) {
            case 1:
                add_student(); // 학생 정보 추가
                break;
            case 2:
                search_student(); // 학생 정보 검색
                break;
            case 3:
                export_students(); // 학생 정보 내보내기
                break;
            case 4:
                update_student(); // 학생 정보 수정
                break;
            case 5:
                delete_student(); // 학생 정보 삭제
                break;
            case 6:
                printf("프로그램을 종료합니다.\n");
                exit(0); // 프로그램 종료
            default:
                printf("잘못된 입력입니다. 다시 선택해 주세요.\n");
        }
    }

    return 0; // 프로그램 종료
}
