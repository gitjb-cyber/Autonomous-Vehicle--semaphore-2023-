# 자율 주행 신호기
- 배경
   - 차세대 AI자율주행 기술이 생겨나면서 사람에게 필요한 주행 기술이 점점 줄어들 것으로 예상
   - 그에 따른 교통기술도 변화.
   - 현재도 미국에 일부 지역에서 V2I(Vehicle-to-Infrastructure) 통신으로 사람과 기계 모두가 인식할 수 있는 도로를 제작 진행중.
   - 도로교통에 가장 보이기 쉬운 신호등과 표지판부터 바뀌게 될 것.
- 목적
  - 신호등을 시각적 신호가 아닌, 차량에 직접 신호를 주는 신호기를 개발.
  - 다수의 차가 한 신호에 멈추는 것에서 각각 온 순서대로 신호를 주고,
  - 신호에 맞게 차들이 도로를 빠르게 빠져나갈 수 있다면,
  - 교통체증도 줄어들고 도로의 급박한 상황을 인지하여 바로 신호를 바꿔 멈추면 인명사고도 크게 줄어들 것.
 
![image](https://github.com/user-attachments/assets/534d4df0-e2a1-4102-b6e6-6bfbf71722f0)
![image](https://github.com/user-attachments/assets/46ab42db-f52b-4519-8e85-7cb6691536c1)

## NRF24L01 통신 모듈을 사용한 실험
## 실험 1 : 1:1 양방향 통신
![image](https://github.com/user-attachments/assets/d2817bca-b33d-44bd-bd82-43e0055b2194)
![image](https://github.com/user-attachments/assets/b6392a4e-aa37-4df2-8a31-00170699fb0d)

### 코드 설명(메인 모듈)
![image](https://github.com/user-attachments/assets/90d8cd76-d330-44e2-93f3-916e92fb951a)
![image](https://github.com/user-attachments/assets/e1db1499-2e2b-498b-b4b4-aec44aee4408)
![image](https://github.com/user-attachments/assets/8d16eca8-b8d7-42e2-a0ad-be3ffbccef65)
![image](https://github.com/user-attachments/assets/19a49d58-ff37-46b2-bb3e-0d4b1c401be2)
![image](https://github.com/user-attachments/assets/4f77ef72-76c8-4851-aac8-ba7535531db1)
![image](https://github.com/user-attachments/assets/3eb70d37-cfeb-4b18-ada6-27f0791f4a04)

### 코드 설명(서브 모듈)
![image](https://github.com/user-attachments/assets/1d0a7c85-ff90-4875-a6b8-cc0c81e7fb2b)
![image](https://github.com/user-attachments/assets/71c2d117-c930-4653-895e-9fedcdb5f2fe)
![image](https://github.com/user-attachments/assets/ffb31b29-6be2-4658-bbf1-6ba74dab8f24)
![image](https://github.com/user-attachments/assets/9fae3693-a146-45d3-8384-cce6c31d5dea)
![image](https://github.com/user-attachments/assets/3d1f55b0-97d7-4f66-a3cd-357bc7bc2816)
![image](https://github.com/user-attachments/assets/878c9e0c-3db0-4693-8820-3104e7caabaa)
![image](https://github.com/user-attachments/assets/ab843ec6-7b35-4e04-8aa7-624ff97b27e9)

### 실제 적용 실험
![image](https://github.com/user-attachments/assets/14312202-db12-4ab5-9a35-7d3a6a094db8)
![image](https://github.com/user-attachments/assets/ee016377-582f-4c42-ace3-f75c7326a379)
![image](https://github.com/user-attachments/assets/5d1011e3-b276-4a0f-a977-757d2ea2e5bc)


## 실험 2 : 1:2 양방향 통신
![image](https://github.com/user-attachments/assets/f319d405-9bf2-407a-9737-022b4c878421)

### 코드 설명(메인 오더)
![image](https://github.com/user-attachments/assets/1a905086-ddf1-42b7-97e8-6fdc03d06b0f)
![image](https://github.com/user-attachments/assets/bf73d128-0287-48e0-ae80-74aa14eddd09)
![image](https://github.com/user-attachments/assets/7377d9d4-6ccc-41ba-a46b-69bde5581c30)
![image](https://github.com/user-attachments/assets/8b981bd6-94b1-471f-978d-1d0d6b188228)

### 코드 설명(모듈 A, B)
![image](https://github.com/user-attachments/assets/8ae22b5a-7550-4135-86ef-fbd6f6f91054)
![image](https://github.com/user-attachments/assets/e3f2275d-6cb8-4930-af62-e1705465f41b)

### 실제 적용 실험
![image](https://github.com/user-attachments/assets/19acb3ea-dba0-4273-9532-571ff67406c5)
![image](https://github.com/user-attachments/assets/1e303517-35bd-4511-a0e5-23516d21580a)
![image](https://github.com/user-attachments/assets/175836fd-d035-44cb-9037-b9698cc8646c)
![image](https://github.com/user-attachments/assets/a32f7f6a-53b9-4939-aff5-1c95c8cdc06c)


























