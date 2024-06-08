# control-server

Visualize the amount of logs per host<br>
<img width="1000" alt="파이썬시각화시간별 호스트로그" src="https://github.com/wngh1212/control-server/assets/88926634/9251d5c0-efea-457a-b2bf-d41ea7af1cbf">


Since it is still in its initial state, the code has exposed all of the sql access information, DB name, and table name

It's not a daemon, but I combined the binary that compiled the cscript with the shellscript to make it work like a daemon<br>
log_loop.sh &

Parse every 5 minutes, push it into the database, remove the syslog file, recreate it, and restart the syslog daemon

In regular expression (regex):

Unique ID | Date | Time | Hostname | Program Name | Program PID | log message<br>
Identifier<br> added because PID is not unique<br>

Code refactoring part: DB connection information and queries are exposed on the code as is <br>

The actual working client part will be produced separately

- Issues and add-ons
- Information error due to different time zones for different servers (medium)
- sql access information code exposed (high)
- I have to schedule it to run shell scripts automatically when the server suddenly dies or turns on to prepare for a reboot... (high)
- Create and compress data files (csv,json) in the database every day, and delete from syslog; (low) the existing database
- Existing date and time were put in the same field but separated (complete)
- Not searching for kernel at all (low)
- Error causing error by entering unscaped string (complete)
- Value entered oddly due to incorrect data segmentation (complete)
- Search, filter, create a part of the client for meaningful data processing (unknown)
* 

kill -l
Check the process or system status of other servers through the signal..?

---

호스트별 로그 양 시각화<br>
<img width="1000" alt="파이썬시각화시간별 호스트로그" src="https://github.com/wngh1212/control-server/assets/88926634/9251d5c0-efea-457a-b2bf-d41ea7af1cbf">

아직 초기 상태이기 때문에 코드에 sql접속 정보랑 DB명, 테이블 이름 다 노출 되있음

데몬은 아니지만 c스크립트를 컴파일한 바이너리와 쉘스크립트를 결합하여 데몬처럼 작동하게 놔둠<br>
log_loop.sh &

5분마다 파싱해서 데이터베이스에 밀어 넣고 syslog파일 제거 후 재 생성 및 syslog 데몬 재시작

정규표현식(regex)로 : 

고유ID | 날짜 | 시간 | 호스트이름 | 프로그램 이름 | 프로그램 PID | log message<br>
PID는 unique하지 않기 때문에 추가한 식별자<br>

코드 리팩터링 부분 : 코드 상에 DB 접속 정보 및 쿼리가 그대로 노출 되어 있음<br>

실제로 작동하는 클라이언트 부분은 따로 제작 예정

- 이슈 및 추가 기능 
    - 서버별로 시간대가 다름으로 생기는 정보의 오류 (medium)
    - sql 접속 정보 코드 노출 ( high )
    - 갑자기 서버가 죽거나 재부팅을 대비하기 위해 켜질 때 자동으로 쉘 스크립트 실행하게 스케줄에 올려야 하는데… (high)
    - 데이터베이스의 데이터를 하루마다 데이터파일(csv,json) 생성 및 압축하고 기존의 데이터베이스는 delete from syslog; (low)
    - 기존의 날짜와 시간은 같은 필드에 넣었지만 분리하였음 (complete)
    - 커널에 대한 서치가 전혀 되고 있지 않음 (low)
    - 이스케이프 처리 안된 문자열 들어가서 에러 일으키는 오류 ( complete )
    - 데이터 분할이 잘 못 되서 값이 이상하게 들어감 (complete)
    - 검색,필터링 유의미한 데이터 처리를 위한 클라이언트 부분 제작 예정 ( unknown )
*

kill -l
시그널을 통해서 다른 서버의 프로세스나 시스템 상태를 체크..?
