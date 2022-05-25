# 3주차 (21. 11. 01)
[Notion Link](https://www.notion.so/3-21-11-01-1480299a3c014c219b551ec7b9b3a477)

### 3차원 표현

원근법 : 멀고 가까운 거리감을 느낄 수 있도록 표현하는 기법

물체 겹침 : 불투명한 물체의 일부 또는 전체가 보이는 것

조명, 그림자

### 모형의 표현

일반적인 그래픽스 프로그램에서 3차원 물체를 메시로 근사해서 표현한다.

(삼각형을 이용해서 물체를 표현한다. EX. 3DMax, Maya, 

### 색 공간(Color Space)

CIE도표에서 표현하고자 하는 색상을 표현하기 위해 어느 범위를 사용할 지에 대한 포맷

### 렌더링 파이프 라인

3차원 장면의 모습에 따라서 2차원 이미지를 생성하는데 필요한 일련의 단계들

<aside>
💡 입력 조립기 단계 → 정점 셰이더 → 덮개 셰이더 → 테셀레이터 → 영역 셰이더 → 기하 셰이더 → 레스터화 → 픽셀 셰이더 → 출력 병합기

</aside>

테셀레이터는 언리얼에서 사용할 수 없다.
C++에서는 개조시킬 수 있다.

오히려 기하셰이더를 많이 사용한다. 주로 파티클 만들 때 유용하다.