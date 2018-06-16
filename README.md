# Cantata 조

## 2018-1학기 OSSP



## 팀원

- 2013111999 김성준
- 2014112062 양준현
- 2014112032 최동혁



## Description

* 동국대학교 컴퓨터공학과 공개SW프로젝트 2018-1 학기 프로젝트 원격저장소.
  * 리눅스 SDL을 이용한 Pacman Game
  * 소스코드 출처
    * Pacman_original  : <https://github.com/TroyShaw/Pacman>  
    * Pacman_2017_ver : https://github.com/CSID-DGU/2017-2-OSSP-Awkward-3



## 협업 홈페이지

* http://13.125.138.114

<img src="https://i.imgur.com/RmcJZ05.png">





## Getting Started

### 1. Install SDL

```
sudo apt-get update
```

* 필요한 SDL 라이브러리 설치

```
sudo apt-get install libsdl-ttf2.0-0 libsdl-sound1.2 libsdl-mixer1.2
```

```
sudo apt-get install libsdl-image1.2 libsdl-gfx1.2-5 libsdl1.2-dev
```

### 2. Compile

* 먼저 make 파일이 있는 폴더로 이동함
  ```ㅁㄴㅇㅁㅇ 
  cd pacman
  ```

* make 파일을 이용하여 컴파일

  ```
  make
  ```

### 3. Execute

* 게임파일 실행한다.

  ````
  ./game_Debug
  ````



## How to Play Online Mode

### 1. Online Mode로 들어간다.

<img src="https://i.imgur.com/dUnoqcZ.png">

<center>맨 처음 실행시 보이는 화면 / ONLINE 에 커서를 두고 ENTER 를 누른다.</center>

### 2. 먼저 방을 만든다.

<img src="https://i.imgur.com/KIFCRXe.png">

<center>메뉴에서 ONLINE으로 접속시 보이는 화면</center>

<img src="https://i.imgur.com/q6efYLy.png">

<center>MAKE ROOM 선택 후 보이는 화면</center>

### 3.방을 만든 PC의 IP를 입력하고 다른 사람이 접속을 한다.

<img src="https://i.imgur.com/P9jAkpH.png">

<center>커서를 JOIN ROOM 에 두고 IP 를 입력한다. </center>



## License

This project is licensed under the MIT License - see the [pacman/LICENSE.md](/pacman/LICENSE) file for details
