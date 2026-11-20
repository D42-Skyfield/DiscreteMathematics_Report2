#include <iostream>
using namespace std;

class BooleanMatrix{//
public:
    BooleanMatrix(){
        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 5; j++){
                matrix[i][j] = false;
            }
        }
    }//배열은 기본값으로 초기화

    BooleanMatrix* booleanMultiply(BooleanMatrix* other){
        BooleanMatrix* result = new BooleanMatrix();
        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 5; j++){
                for(int k = 0; k < 5; k++){
                    if(this->matrix[i][k] && other->matrix[k][j]){
                        result->matrix[i][j] = true;
                        break;
                    }
                }
            }
        }
        return result;
    }

    void inputMatrix(){//행렬 입력
        int temp; //관계행렬 입력이 정수 0과 1일 것으로 예상되므로 int 변수 temp에 입력을 받는다.
        for(int i = 0; i < 5; i++){
            cout << "row " << i << " : ";
            for(int j = 0; j < 5; j++){
                cin >> temp;
                if(temp == 0 || temp == 1){ //입력이 0, 1인 경우
                    this->matrix[i][j] = (bool)temp; //temp를 boolean으로 typecasting한다. temp가 1이면 true, 0이면 false로 typecasting된다.
                }
                else{ // 2 등 boolean행렬을 만족하지 않는 입력에 대한 예외 처리
                    cout << "Invalid value : " << temp << endl; //에러 메세지
                    cout << "Type value again(input value should be 0 or 1)" << endl;
                    j--; //j값을 1 감소 시켜 잘못 입력한 위치에서 다시 입력받도록 함.
                }
            }
            cout << '\n';
        }
    }
    
    void display(){//행렬 console출력
        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 5; j++){
                switch(i){
                    case 0:
                        if(j == 0)
                            cout << "/ "<< this->matrix[i][j]<<" ";
                        else if(j == 4)
                            cout << this->matrix[i][j]<<" \\\n";
                        else
                            cout << this->matrix[i][j]<<" ";
                        break;
                    case 4:
                        if(j == 0)
                            cout << "\\ "<< this->matrix[i][j]<<" ";
                        else if(j == 4)
                            cout << this->matrix[i][j]<<" /\n";
                        else
                            cout << this->matrix[i][j]<<" ";
                        break;
                    default:
                    if(j == 0)
                            cout << "| "<< this->matrix[i][j]<<" ";
                        else if(j == 4)
                            cout << this->matrix[i][j]<<" |\n";
                        else
                            cout << this->matrix[i][j]<<" ";
                        break;
                }    
            }
        }
    }

    bool matrix[5][5]; //boolean값은 true, false만 가능하다.
};

class RelationMatrix : public BooleanMatrix{
    public:
    RelationMatrix(){}//default 생성자

    const int cardinality = 5; //현재의 구현에서 집합A의 cardinality는 5로 고정된 상수값이다.

    bool checkEquivalance(){ //동치인지 비교
        bool factor1 = checkReflective();
        bool factor2 = checkSymmetric();
        bool factor3 = checkTransitive();
        return factor1 && factor2 && factor3; //셋 중 하나라도 false이면 false를 리턴한다.
    }

    bool checkReflective(){ //반사관계인지 확인
        for(int i = 0; i < cardinality; i++){//대각성분에 대하여
            if(!(this->matrix[i][i])) return false; //true가 아닌 성분이 하나라도 포함되어 있다면 반사관계가 아니다.
        }
        return true; //모든 대각성분이 true이면 반사관계의 조건을 만족한다.
    }

    bool checkSymmetric(){//대칭행렬인지 확인
        for(int i = 0; i < cardinality; i++){ //하삼각행렬을 순회할 경우 중복 순회가 되므로 for문의 순회 범위는 상삼각행렬만 순회 대상으로 한다. 대각성분은 대칭관계에 영향을 주지 않으므로 순회 대상에서 제외한다.
            for(int j = i + 1; j < cardinality; j++){
                if(this->matrix[i][j] != this->matrix[j][i]) return false;//대칭되는 위치에 있는 성분과 값이 다른 경우가 하나라도 있다면 대칭행렬이 아니다.
            }
        }
        return true;
    }

    bool checkTransitive(){//추이관계인지를 합성관계를 통해 확인
        BooleanMatrix* MRi = this; //R^i(i는 n 이하의 natural number)에 대한 관계행렬이다.
        BooleanMatrix* temp;
        //n이 1일 때 R ^ 1 == R이므로 R ^ 1은 R의 부분집합임은 자명하다.
        for(int i = 2; i <= cardinality; i++){ //2부터 cardinality까지..
            temp = this->booleanMultiply(MRi);//R ^ i의 관계행렬을 구한다. 메모리 낭비를 막기 위해 임시 포인터 변수를 활용하여 동적 할당한다.
            if(MRi != this)delete MRi; //this가 아닌 경우 MR이 가리키는 객체 메모리 반환. R의 주소값을 바꾸기 전 Garbage에 대한 처리이다.
            MRi = temp;
            for(int j = 0; j < cardinality; j++){
                for(int k = 0; k < cardinality; k++){
                    if(MRi->matrix[j][k]){ //R ^ i의 j행k열이 1인 경우만 따진다. 0인 경우는 판별 대상이 아니다. 
                        if(!this->matrix[j][k]) return false; //R의 관계행렬(this->matrix)의 동일 위치의 성분이 0이라면 R ^ i는 R의 부분집합이 성립하지 않는다. 이 경우 추이관계가 아니다(n 이하 자연수 k에 대해 R^k가 R의 부분집합이 아니면 R은 관계 A에 대한 추이관계가 아니다.)
                    }
                }
            }
        }
        return true;
    }

    void equivalanceClass(){//동치류 출력 함수. 출력 과정에서 동치류 판별을 내부적으로 실시한다.
        /* 출력 형식 : "[i] : {1, 2, 3, 4, 5}"*/
        for(int i = 0; i < cardinality; i++){//i의 순회 범위는 0 ~ 4이지만, 출력되는 동치류는 [1] ~ [5] 까지이다.
            cout << "[" << i + 1 << "]: {  "; 
            bool hasElemnt = false;//빈 집합의 중괄호가 반대로 출력되는 버그를 해결하기 위함.
            for(int j = 0; j < cardinality; j++){
                if(this->matrix[i][j]){ //동치류 [i]의 원소는 관계행렬의 i행에서 값이 1인 원소의 열 번호 j와 같다.
                    if(!hasElemnt){
                        cout << "\b\b";
                    }
                    hasElemnt = true;
                    cout << j + 1<<", "; //동치류의 원소 출력. j의 순회 값 범위가 0 ~ 4이므로 1을 더하여 1 ~ 5 범위의 값을 출력하게 한다.
                }
            }
            cout << "\b\b}"<<endl;
        }
    }

    void printRelation(){//관계행렬로부터 관계를 복원하여 집합 형식으로 출력.
        /* 출력 형식 : "Relation : {(1, 1), (1, 3)}" */
        cout << "Relation : {  ";
        bool hasElemnt = false;//빈 집합의 중괄호가 반대로 출력되는 버그를 해결하기 위함.
        for(int i = 0; i < cardinality; i++){
            for(int j = 0; j < cardinality; j++){
                if(this->matrix[i][j]){
                    if(!hasElemnt){
                        cout << "\b\b";
                    }
                    hasElemnt = true;
                    cout << "("<< i + 1 <<", "<< j + 1 <<"), ";
                }
            }
        }
        cout << "\b\b}" << endl;
    }

    RelationMatrix* reflectiveClosure(){//반사폐포를 동적 생성하여 동적 할당 후 주소값을 반환
        if(checkReflective()) return this; //이미 반사관계이이므로 반사폐포는 자기 자신이다.
        RelationMatrix* result = new RelationMatrix();
        for(int i = 0; i < cardinality; i++){
            for(int j = 0; j < cardinality; j++){
                if(i==j){
                    result->matrix[i][j] = 1; //대각성분이면 무조건 1로 만들어버린다.
                }
                else{
                    result->matrix[i][j] = this->matrix[i][j];
                }
            }
        }
        return result;
    } 
    
    RelationMatrix* symmetricClosure(){//대칭폐포를 동적 생성하여 동적 할당 후 주소값을 반환
        if(checkSymmetric()) return this; //이미 대칭관계이므로 대칭폐포는 자기 자신이다.
        RelationMatrix* result = new RelationMatrix();
        for(int i = 0; i < cardinality; i++){
            for(int j = 0; j < cardinality; j++){
                if(this->matrix[i][j]){
                    result->matrix[i][j] = true; //원래 단위행렬의 성분과
                    result->matrix[j][i] = true; //단위행렬에서 대칭되는 성분을 모두 true로 표시한다.
                }
            }
        }
        return result;
    } 
    
    RelationMatrix* transitiveClosure(){//추이폐포를 동적 생성하여 동적 할당 후 주소값을 반환
        if(checkTransitive()) return this; //이미 추이관계이므로 추이폐포는 자기 자신이다.
        RelationMatrix* result = new RelationMatrix();
        for(int k = 0; k <= cardinality; k++){ //와샬 알고리즘을 적용한다. 기존 행렬 복사를 위해 k에 0단계를 추가했다.
            for(int i = 0; i < cardinality; i++){
                for(int j = 0; j < cardinality; j++){
                    if(k == 0){ //k가 0단계일 때 기존의 행렬을 복사한 후,
                        result->matrix[i][j] = this->matrix[i][j];
                    }
                    else{ //k가 1~5단계일 때 와샬 알고리즘을 적용한다.
                        if(!this->matrix[i][j] && this->matrix[i][k] && this->matrix[k][j]) //이 조건이 만족될 때
                        {
                            result->matrix[i][j] = true; //결과행렬의 i행 j열 성분을 true로 추가한다.
                        }
                    }
                }
            }
        }
        return result;
    }
};

bool showEquivalance(RelationMatrix* M){ //동치여부를 판정해 출력한 후 동치 여부의 진리값 반환
    bool equivalant = M->checkEquivalance();
    if(equivalant){//동치이면
        cout << "Relation is Equivalant" << endl; //동치일 때의 메세지 출력
    }
    else{
        cout << "Relation is not Equivalant" << endl; //동치가 아닐 때의 메세지 출력
    }
    //추가 기능 구현 : 반사성, 대칭성, 추이성 판정 결과
    cout << "Reflexivity: " << M->checkReflective() << endl;
    cout << "Symmetry: " << M->checkSymmetric() << endl;
    cout << "Transivity: " << M->checkTransitive() << endl;
    return equivalant;
}

int main(){ //메인 함수
    RelationMatrix* RM = new RelationMatrix(); //관계 R에 대한 관계행렬 준비
    RM->inputMatrix();//관계행렬 입력기능 실행
    
    RM->display();//입력한 행렬 다시 출력

    bool equivalant = showEquivalance(RM);
    if(equivalant){
        RM->equivalanceClass();
    }
    else{
        cout << "[BEFORE]" << endl;//폐포 구현 전 원래의 관계 출력
        RM->printRelation(); //폐포 변환 전의 관계 출력
        //각각의 폐포를 나타내는 관계행렬 구하기
        RelationMatrix* closure1 = RM->reflectiveClosure();
        RelationMatrix* closure2 = RM->symmetricClosure();
        RelationMatrix* closure3 = RM->transitiveClosure();
        cout << "[Reflective Closure]" << endl;//반사폐포 출력 및 처리부
        closure1->printRelation(); //관계 출력
        if(showEquivalance(closure1)){ //동치여부 판정, 출력을 조건문 내에서 실행하고, 참인 경우 동치류 출력
            closure1->equivalanceClass();
        }
        cout << "[Symmetric Closure]" << endl;//반사폐포와 동일한 로직
        closure2->printRelation();
        if(showEquivalance(closure2)){
            closure2->equivalanceClass();
        }
        cout << "[Transitive Closure]" << endl;//반사폐포와 동일한 로직
        closure3->printRelation();
        if(showEquivalance(closure3)){
            closure3->equivalanceClass();
        }
    }
    int exit; //위의 로직이 실행된 후 프로그램이 꺼지지 않고 유지되게 하기 위한 더미 변수
    cin >> exit; //값을 입력하지 않으면 프로그램은 종료되지 않고 콘솔창이 유지됨.
    return 0;
}