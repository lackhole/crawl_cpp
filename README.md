# crawl_cpp
C++ web crawl


## Requirements
* CMake
* [libcurl](https://curl.se/libcurl/) 
* [libxml2](http://xmlsoft.org/)

### Example output
* You can ignore parse errors  

#### https://naver.com response
```
<html lang='ko' data-dark='false'>
  <head>
    <meta charset='utf-8'></meta>
    <title>
      NAVER
    </title>
    <meta http-equiv='X-UA-Compatible' content='IE=edge'></meta>
    <meta name='viewport' content='width=1190'></meta>
    <meta name='apple-mobile-web-app-title' content='NAVER'></meta>
    <meta name='robots' content='index,nofollow'></meta>
    <meta name='description' content='네이버 메인에서 다양한 정보와 유용한 컨텐츠를 만나 보세요'></meta>
    <meta property='og:title' content='네이버'></meta>
    <meta property='og:url' content='https://www.naver.com/'></meta>
    <meta property='og:image' content='https://s.pstatic.net/static/www/mobile/edit/2016/0705/mobile_212852414260.png'></meta>
    <meta property='og:description' content='네이버 메인에서 다양한 정보와 유용한 컨텐츠를 만나 보세요'></meta>
    <meta name='twitter:card' content='summary'></meta>
    <meta name='twitter:title' content=''></meta>
    <meta name='twitter:url' content='https://www.naver.com/'></meta>
    <meta name='twitter:image' content='https://s.pstatic.net/static/www/mobile/edit/2016/0705/mobile_212852414260.png'></meta>
    <meta name='twitter:description' content='네이버 메인에서 다양한 정보와 유용한 컨텐츠를 만나 보세요'></meta>
    <link rel='stylesheet' href='https://pm.pstatic.net/dist/css/nmain.20201119.css'></link>
    <link rel='stylesheet' href='https://ssl.pstatic.net/sstatic/search/pc/css/sp_autocomplete_201028.css'></link>
    <link rel='shortcut icon' type='image/x-icon' href='/favicon.ico?1'></link>
    <script>
      document.domain="naver.com",window.nmain=window.nmain||{},window.nmain.supportFlicking=!1;var nsc="navertop.v4",ua=navigator.userAgent;window.nmain.isIE=navigator.appName&&navigator.appName.indexOf("Explorer")>0&&ua.toLocaleLowerCase().indexOf("msie 10.0")<0,document.getElementsByTagName("html")[0].setAttribute("data-useragent",ua),window.nmain.isIE&&(Object.create=function(n){function a(){}return a.prototype=n,new a})
    </script>
    <script>
      var darkmode= false;window.naver_corp_da=window.naver_corp_da||{main:{}},window.naver_corp_da.main=window.naver_corp_da.main||{},window.naver_corp_da.main.darkmode=darkmode
    </script>
    <script>
    </script>
    <script>

...
```

#### xpath example output
```
<a href='http://news.naver.com/main/list.nhn?mode=LPOD&mid=sec&sid1=001&sid2=140&oid=001&isYeonhapFlash=Y' class='link_media' data-clk='newsflash'>연합뉴스</a>
<a href='http://news.naver.com/main/list.nhn?mode=LPOD&mid=sec&sid1=001&sid2=140&oid=001&isYeonhapFlash=Y&aid=0012099139' class='issue' data-clk='quickarticle'>정경심 15개 혐의 중 11개 유죄…총장 직인 파일에 발목</a>
<a href='http://news.naver.com/main/list.nhn?mode=LPOD&mid=sec&sid1=001&sid2=140&oid=001&isYeonhapFlash=Y&aid=0012099345' class='issue' data-clk='quickarticle'>오늘 오후 9시까지 911명 확진…내일도 1천명 안팎 나올듯</a>
<a href='http://news.naver.com/main/list.nhn?mode=LPOD&mid=sec&sid1=001&sid2=140&oid=001&isYeonhapFlash=Y&aid=0012099354' class='issue' data-clk='quickarticle'>미 정부 "화이자 백신 1억회분 추가 구매…내년 7월까지 받기로"</a>
<a href='http://news.naver.com/main/list.nhn?mode=LPOD&mid=sec&sid1=001&sid2=140&oid=001&isYeonhapFlash=Y&aid=0012099196' class='issue' data-clk='quickarticle'>'월성원전 자료삭제' 산업부 공무원 3명 법정에…사건 첫 기소</a>
<a href='http://news.naver.com/main/list.nhn?mode=LPOD&mid=sec&sid1=001&sid2=140&oid=001&isYeonhapFlash=Y&aid=0012099353' class='issue' data-clk='quickarticle'>변창흠 "신공항 장기표류로 발전 저해…조속히 입지 정해야"</a>
<a href='http://news.naver.com/main/list.nhn?mode=LPOD&mid=sec&sid1=001&sid2=140&oid=001&isYeonhapFlash=Y&aid=0012099288' class='issue' data-clk='quickarticle'>경찰, '박원순 휴대폰' 포렌식 완료…사망경위 파악 주력</a>
<a href='http://news.naver.com/main/list.nhn?mode=LPOD&mid=sec&sid1=001&sid2=140&oid=001&isYeonhapFlash=Y&aid=0012099117' class='issue' data-clk='quickarticle'>安, 국힘 입당? "논의해볼 수 있지만 선거승리의 방법인가"</a>
<a href='http://news.naver.com/main/list.nhn?mode=LPOD&mid=sec&sid1=001&sid2=140&oid=001&isYeonhapFlash=Y&aid=0012099330' class='issue' data-clk='quickarticle'>홍준표 "MB 면회나 가라"…박형준 "후배 책잡기 그만"</a>
<a href='http://news.naver.com/main/list.nhn?mode=LPOD&mid=sec&sid1=001&sid2=140&oid=001&isYeonhapFlash=Y&aid=0012098778' class='issue' data-clk='quickarticle'>고시원·쪽방 청년에 공공임대 우선공급…이사비도 지원</a>
<a href='http://news.naver.com/main/list.nhn?mode=LPOD&mid=sec&sid1=001&sid2=140&oid=001&isYeonhapFlash=Y&aid=0012099128' class='issue' data-clk='quickarticle'>전 국민 고용보험 로드맵 제시…보험료 분담 등 논란 예상</a>
```