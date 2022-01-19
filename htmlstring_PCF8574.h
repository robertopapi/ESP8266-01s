const static char HTML_head[] PROGMEM = R"~~(
<!DOCTYPE html>
<html lang="en">
<head>
<title>Prova PCF8574</title>
<meta name="format-detection" content="telephone=no">
<meta charset="UTF-8">
<meta  name="viewport" content="width=device-width,initial-scale=1,user-scalable=no"/>
<style>
body {font-family: "Arial", sans-serif; display: block; margin: auto;}
h1 {margin: 50px auto 30px;}
.c
{
 margin-left: auto;
 margin-right: auto;
 text-align: center;
}
.vh {visibility: hidden;}
.rbutton
{
 width:48px;
 height:48px;
 background-color:lightgray;
 border-radius: 24px;
 border-style: solid;
 border-width: 1px;
 border-color: gray;
 cursor:default;
}
.rbutton:hover
{
 box-shadow: 1px 1px 1px 1px #666666;
 -webkit-box-shadow: 1px 1px 1px 1px #666666;
 -moz-box-shadow: 1px 1px 1px 1px #666666;
}
.rbutton:focus
{
 box-shadow: 1px 1px 1px 1px #666666;
 -webkit-box-shadow: 1px 1px 1px 1px #666666;
 -moz-box-shadow: 1px 1px 1px 1px #666666;
}
.rbutton:active
{
 box-shadow: 1px 1px 1px 1px #666666 inset;
 -webkit-box-shadow: 1px 1px 1px 1px #666666 inset;
 -moz-box-shadow: 1px 1px 1px 1px #666666 inset;
}
.rbutton.on
{
 background-color:gray;
 box-shadow: 1px 1px 1px 1px #666666 inset;
 -webkit-box-shadow: 1px 1px 1px 1px #666666 inset;
 -moz-box-shadow: 1px 1px 1px 1px #666666 inset;
}
.led
{
 width:32px;
 height:32px;
 border-radius: 16px;
 border-style: solid;
 border-width: 1px;
 border-color: gray;
 background-color:lightgray;
 cursor:default;
}
.led:hover
{
 background-repeat: no-repeat;
 background-position: left 0px top 0px;
 box-shadow: 1px 1px 1px 1px #666666;
 -webkit-box-shadow: 1px 1px 1px 1px #666666;
 -moz-box-shadow: 1px 1px 1px 1px #666666;
}
.led:focus
{
 background-repeat: no-repeat;
 background-position: left 0px top 0px;
 box-shadow: 1px 1px 1px 1px #666666;
 -webkit-box-shadow: 1px 1px 1px 1px #666666;
 -moz-box-shadow: 1px 1px 1px 1px #666666;
}
.led:active
{
 background-repeat: no-repeat;
 background-position: left 0px top 0px;
 box-shadow: 1px 1px 1px 1px #666666 inset;
 -webkit-box-shadow: 1px 1px 1px 1px #666666 inset;
 -moz-box-shadow: 1px 1px 1px 1px #666666 inset;
}
.led.on
{
 background-repeat: no-repeat;
 background-position: left 0px top 0px;
 background-color:lime;
}
.buzzer
{
 width:28px;
 height:28px;
 border-radius: 14px;
 border-style: solid;
 border-width: 1px;
 border-color: gray;
 background-repeat: no-repeat;
 background-position: left 0px top 0px;
 background-image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABwAAAAcCAYAAAByDd+UAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsMAAA7DAcdvqGQAAAAGYktHRAD/AP8A/6C9p5MAAAAHdElNRQfeCwwXBjnJ0MMtAAAAHWlUWHRDb21tZW50AAAAAABDcmVhdGVkIHdpdGggR0lNUGQuZQcAAADiSURBVEhL7Y/BDsIwDEP3/z89FglLL06bbiD1gHiXxnNiw3Fu5l845Dg+/52PL6NsW6GKthSypCsMr/Xfb4sHdIFBV7os5KHmUZiXuBb1C/AA0c2dDrK60JIfEvdm2r0gKT/kS0rIpbk/mkVSNH2RlBAEz2aRFE1fJCUEwbNZJEXTF0kJQfBsFknR9EVSQhA8m0VSNHnklJBLc380i5KmJS6WI/Nm2r0gK8ODRDd3OshqgAfwJR7uWtQvA/xwFERmZcGtwoABXWFXFtwuDBTUBa54fLn6Bys+utxe+A2/XnieL9tRo+k2UNHwAAAAAElFTkSuQmCC);
 cursor:default;
}
.buzzer:hover
{
 box-shadow: 1px 1px 1px 1px #666666;
 -webkit-box-shadow: 1px 1px 1px 1px #666666;
 -moz-box-shadow: 1px 1px 1px 1px #666666;
}
.buzzer:focus
{
 box-shadow: 1px 1px 1px 1px #666666;
 -webkit-box-shadow: 1px 1px 1px 1px #666666;
 -moz-box-shadow: 1px 1px 1px 1px #666666;
}
.buzzer:active
{
 box-shadow: 1px 1px 1px 1px #666666 inset;
 -webkit-box-shadow: 1px 1px 1px 1px #666666 inset;
 -moz-box-shadow: 1px 1px 1px 1px #666666 inset;
}
.buzzer.on
{
 background-image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABwAAAAcCAYAAAByDd+UAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsMAAA7DAcdvqGQAAAAGYktHRAD/AP8A/6C9p5MAAAAHdElNRQfeCwwXBjnJ0MMtAAAAHWlUWHRDb21tZW50AAAAAABDcmVhdGVkIHdpdGggR0lNUGQuZQcAAAEESURBVEhLvY0JDoQwDAP5/6dZWjHIMW6BlWAk0xjnWNaPefXgsiyH4LWDekyP/nWQ4RF+CPWsfx+gwwk94Op5/97kGNpfR5e74PbBMiQ16HKXcuvgaSj4kZzLgzpE7f9GSkwP6lCqWZykuVLdhg81fKihfS5w3yjOm/VVWJREzksNxWnojcCSJM29huI09MYGC5JAvWeN4nxQYTiJnDfVUJyGXo+k+ayG4jT0oSRQP6qhuo00kAQz71mjOoHmJEW957F/fws0JpE7mjfcw+kPjUmgdcL7lfKXxiTFvZL6lSOhMQmo9d9T+qQudzmj/3eZHhwxy64YHnyLY/MXxxrvbj+xrj8VCihXEHb5jQAAAABJRU5ErkJggg==);
}
</style>
</head>
)~~";


const static char HTML_body[] PROGMEM = R"~~(
<body>
<center>
<h1>Prova PCF8574</h1>
<table>  
  <tr>
   <td style="padding 2px 2px;"><div class="led c" id="l1" name="l1" value="1" onclick="javascript:doclick(this);">L1</div></td>
   <td style="padding 2px 2px;"><div class="led c" id="l2" name="l2" value="1" onclick="javascript:doclick(this);">L2</div></td>
   <td style="padding 2px 2px;"><div class="led c" id="l3" name="l3" value="1" onclick="javascript:doclick(this);">L3</div></td>
   <td style="padding 2px 2px;"><div class="led c" id="l4" name="l4" value="1" onclick="javascript:doclick(this);">L4</div></td>
  </tr>
  <tr>
   <td style="padding 2px 2px;"><div class="rbutton c" id="rb1" name="rb1" value="1" onclick="javascript:doclick(this);">rb1</div></td>
   <td style="padding 2px 2px;"><div class="rbutton c" id="rb2" name="rb2" value="1" onclick="javascript:doclick(this);">rb2</div></td>
   <td style="padding 2px 2px;"><div class="rbutton c" id="rb3" name="rb3" value="1" onclick="javascript:doclick(this);">rb3</div></td>
   <td style="padding 2px 2px;"><div class="buzzer" id="bz1" name="bz1" value="0" onclick="javascript:doclick(this);">&nbsp;</div></td>
  </tr>
</table>
<textarea id="debug" name="debug" rows="30" cols="100"></textarea>
<div id="bresult" name="bresult">&nbsp;</div>
<div id="serverresp">&nbsp;</div>
</center>
)~~";


const static char HTML_script[] PROGMEM = R"~~(
<script type="text/javascript">
<!--
var resp=document.getElementById("serverresp");
var getPeriodicData = "/updatedata";
var datasave = "/datasave";
function v(id)
{
 document.getElementById(id).style.visibility="visible";
}
function h(id)
{
 document.getElementById(id).style.visibility="hidden";
}
function res(obj, id)
{
 v(id);
 document.getElementById(id).innerHTML=obj.value;
}
function isJSON(str)
{
 try
 {
  JSON.parse(str);
 }
 catch (e)
 {
  return false;
 }
 return true;
}
function getData()
{
 var obj;
 var url="/";
 var param="";
 switch (arguments.length)
 {
  case 3:
   param=arguments[2];
  case 2:
   url=arguments[1];
  case 1:
   obj=arguments[0];
   break;
  default:
   return(-1);
 }
 
 debug("getdata");
 debug("url="+url+" param="+param);
 
 var xhttp = new XMLHttpRequest();
 xhttp.onreadystatechange = function()
  {
   if (this.readyState == 4 && this.status == 200)
    obj.innerHTML=this.responseText;
  };
 xhttp.open("POST", url, true);
 if (isJSON(param))
  xhttp.setRequestHeader('Content-Type', 'application/json');
 if (param!="")
  xhttp.send(param);
 else
  xhttp.send();
}
function doclick(obj)
{
 if (obj.value=="0")
  obj.value="1";
 else
  obj.value="0";

 debug("doclick("+obj.id+") value="+obj.value);
  
 sendconf();
}
function refreshview()
{
 refreshitem(document.getElementById("l1"));
 refreshitem(document.getElementById("l2"));
 refreshitem(document.getElementById("l3"));
 refreshitem(document.getElementById("l4"));
 refreshitem(document.getElementById("rb1"));
 refreshitem(document.getElementById("rb2"));
 refreshitem(document.getElementById("rb3"));
 refreshitem(document.getElementById("bz1"));
}
function refreshitem(obj)
{
 if (obj.value=="0")
  obj.classList.add("on");
 else
  obj.classList.remove("on");
}
function loadPeriodicData(txt)
{
 debug("loadPeriodicData("+txt+")");
 
 var ajaxperiodicret = JSON.parse(txt);
 if (ajaxperiodicret != "")
 {
  document.getElementById("l1").value=ajaxperiodicret.l1;
  document.getElementById("l2").value=ajaxperiodicret.l2;
  document.getElementById("l3").value=ajaxperiodicret.l3;
  document.getElementById("l4").value=ajaxperiodicret.l4;
  document.getElementById("rb1").value=ajaxperiodicret.rb1;
  document.getElementById("rb2").value=ajaxperiodicret.rb2;
  document.getElementById("rb3").value=ajaxperiodicret.rb3;
  document.getElementById("bz1").value=ajaxperiodicret.bz1;
  document.getElementById("bresult").innerHTML=ajaxperiodicret.bresult;
  refreshview();
 }
}
function sendconf()
{
 var JSONdata = "{ \"l1\": ";
 if (typeof document.getElementById("l1").value === 'undefined')
 {
  document.getElementById("l1").value = "1";
  JSONdata += "1";
 }
 else
  JSONdata += document.getElementById("l1").value;
	 
 JSONdata += ", \"l2\": ";
 if (typeof document.getElementById("l2").value === 'undefined')
 {
  document.getElementById("l2").value = "1";
  JSONdata += "1";
 }
 else
  JSONdata += document.getElementById("l2").value;

 JSONdata += ", \"l3\": ";
 if (typeof document.getElementById("l3").value === 'undefined')
 {
  document.getElementById("l3").value = "1";
  JSONdata += "1";
 }
 else
  JSONdata += document.getElementById("l3").value;

 JSONdata += ", \"l4\": ";
 if (typeof document.getElementById("l4").value === 'undefined')
 {
  document.getElementById("l4").value = "1";
  JSONdata += "1";
 }
 else
  JSONdata += document.getElementById("l4").value;

 JSONdata += ", \"rb1\": ";
 if (typeof document.getElementById("rb1").value === 'undefined')
 {
  document.getElementById("rb1").value = "1";
  JSONdata += "1";
 }
 else
  JSONdata += document.getElementById("rb1").value;

 JSONdata += ", \"rb2\": ";
 if (typeof document.getElementById("rb2").value === 'undefined')
 {
  document.getElementById("rb2").value = "1";
  JSONdata += "1";
 }
 else
  JSONdata += document.getElementById("rb2").value;

 JSONdata += ", \"rb3\": ";
 if (typeof document.getElementById("rb3").value === 'undefined')
 {
  document.getElementById("rb3").value = "1";
  JSONdata += "1";
 }
 else
  JSONdata += document.getElementById("rb3").value;

 JSONdata += ", \"bz1\": ";
 if (typeof document.getElementById("bz1").value === 'undefined')
 {
  document.getElementById("bz1").value = "0";
  JSONdata += "0";
 }
 else
  JSONdata += document.getElementById("bz1").value;

 JSONdata +=" }";
 resp.innerHTML="";
 debug("sendconf");
 debug(JSONdata);
 getData(resp, datasave, JSONdata);
}
function debug(txt)
{
 if (txt=="")
  document.getElementById("debug").value = "";
 else
  document.getElementById("debug").value = document.getElementById("debug").value+"\n"+txt;
}
var eventSource = new EventSource(getPeriodicData);
///////////////////////////////////////////////////////////////
//eventSource.onmessage = function(e)
// {
//  debug("onmessage: "+e.data);
// }
//eventSource.onopen = function(e)
// {
//  debug("onopen");
// }
///////////////////////////////////////////////////////////////
eventSource.addEventListener("open", function(e)
 {
  debug("receveid open: Una connessione è stata aperta");
 }
);
eventSource.addEventListener("error", function(e)
 {
  if (e.readyState == EventSource.CLOSED)
  {
   debug("receveid error: La connessione è stata chiusa");
  }
 }
);
eventSource.addEventListener("message", function(e)
 {
  debug("received message");
  debug(e.data);
  loadPeriodicData(e.data);
 }
);
//-->
</script>
)~~";

const static char HTML_tail[] PROGMEM = R"~~(</body></html>)~~";
 
