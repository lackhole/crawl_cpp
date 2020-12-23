# crawl_cpp
C++ web crawl


## Requirements
* CMake
* [libcurl](https://curl.se/libcurl/) 
* [libxml2](http://xmlsoft.org/)

### Example output
* You can ignore parse errors  

#### https://naver.com
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