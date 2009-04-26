// eda_events.js -- Apr 23, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

function initEvents()
{
  window.addEventListener("mousewheel",mouseWheelHandler,true);
  window.addEventListener('DOMMouseScroll', mouseWheelHandler, true);
  //window.addEventListener("keypress",handlerKeyPress,true);
  window.onkeydown=handlerKeyPress;
  window.focus();

  mode=1;
  rx=0;
  ry=0;
  moving=0;
  document.getElementById('mega').onmousedown=function(){return false};
  document.getElementById('functionlist').onmousedown=function(){return false};

  window.addEventListener('mousedown', function(e) {
    terminateRename();
    if(e.target.id=="root" ||
        e.target.parentNode.getAttribute("class")=="frame" ||
        e.target.getAttribute("class")=="bg") {
      x=e.clientX;
      y=e.clientY;
      window.addEventListener('mousemove', movescreen, false);
      moving=1;
    }
    return false;
  },false);

  window.addEventListener('mouseup', function(e) {
    if(mode==1) {
      window.removeEventListener('mousemove', movescreen, false);
      if(moving!=1) {
        handleMousedownNavigate(e);
      }
      moving=0;
    }
  },false);

}

//*****************************************************************************
//*******************************KEYBOARD**************************************
//*****************************************************************************

var mode;  //0 is text, 1 is graph
var renaming=null;
var rbox;


var renameOldValue;

function terminateRename() {
  if(renaming!=null) {
    renaming.innerHTML=renameOldValue;
    renaming=null;
  }
}

function handlerKeyPress(e) {
  geohot.debug("got key press: "+String.fromCharCode(e.keyCode)+"("+e.keyCode+")");
  //var skey=String.fromCharCode(e.keyCode)
  //alert(e.keyCode);
  //alert(String.fromCharCode(e.keyCode));
  if(e.keyCode==78 || e.keyCode==110) {  //'n' for rename
    if(renaming==null && selected!=null && (selected.className=="location" || selected.className=="addr")) {
      renaming=selected;
      renameOldValue=selected.innerHTML;
      rbox=document.createElement('input');
      rbox.setAttribute("type","text");
      rbox.setAttribute("id","rename");
      rbox.setAttribute("value",renameOldValue);
      renaming.innerHTML="";
      renaming.appendChild(rbox);
      rbox.focus();
      e.returnValue=false;
    }
  }
  if(e.keyCode==13) {    //enter
    if(renaming!=null && rbox.value.length>0) {
      renaming.innerHTML=rbox.value;
      //push this to server
      sendRename(renameOldValue, renaming.innerHTML);
      renaming=null;
    }
  }
  if(e.keyCode==96) {  //bootleg escape
    terminateRename();
  }
  /*if(e.keyCode==32)
  {
    var el = document.getElementById('mega');
    if(mode==0)  //switch to graph7
    {
      mode=1;
      setStyleByClass("boringbox","codebox");

      el.style.left=rx+"px";
      el.style.top=ry+"px";
    }
    else    //switch to text
    {
      setStyleByClass("codebox","boringbox");
      mode=0;
      el.style.left="0px";
      el.style.top="0px";
    }
  }*/
}

//*****************************************************************************
//*********************************NAVIGATION**********************************
//*****************************************************************************

var oldhash;
setInterval(function() {
  if(oldhash!=null && oldhash!=location.hash && location.hash!="")
    refreshFunction(location.hash.replace("#",""));
  oldhash=location.hash;
}, 50);


var selected=null;
function setSelectedStyle(t) {
  t.style.backgroundColor="orange";
  t.style.color="black";
}

function resetSelectedStyle(t) {
  t.style.backgroundColor="";
  t.style.color="";
}

function handleMousedownNavigate(e) {
  if(selected!=null)
    resetSelectedStyle(selected);

  //alert(e.target.className);
  if(e.target.className=="location" || e.target.className=="addr") {
    selected=e.target;
    setSelectedStyle(selected);
  }
    //alert(e.target.innerHTML);
    //refreshFunction(e.target.innerHTML);
}

window.addEventListener('dblclick', function(e) {
  //alert("got double click");
  if(e.target.className=="location") {
    refreshFunction(selected.innerHTML);
  }
},false);

//*****************************************************************************
//*******************************SCREEN MOVEMENT*******************************
//*****************************************************************************

var glideInterval;

function glide() {
  var ratiox,ratioy;
  var glideRate=50;  //set gliderate here

  if(Math.abs(gx-rx)<=glideRate && Math.abs(gy-ry)<=glideRate) {
    rx=gx;
    ry=gy;
    clearInterval(glideInterval);
  }
  else {
    ratiox=(gx-rx)/Math.abs((gx-rx)+(gy-ry));
    ratioy=(gy-ry)/Math.abs((gx-rx)+(gy-ry));

    rx+=glideRate*ratiox;
    ry+=glideRate*ratioy;
  }

  updateScreen();


}

var gx,gy;
var x,y;
var rx,ry;
var moving;

function mouseWheelHandler(e) {
  //alert("Scroll");
  //alert(navigator.userAgent);
  var ue=navigator.userAgent.toLowerCase()
  if(ue.indexOf("chrome") != -1)
    ry=(ry+e.wheelDelta/120);   //for chrome
  else if(ue.indexOf("firefox") != -1)
    ry=ry-(e.detail*40);         //for firefox
  else
    ry=ry+e.wheelDelta;         //for safari

  updateScreen();
}


function movescreen(e) {
  rx=rx+(e.clientX-x);
  ry=ry+(e.clientY-y);
  x=e.clientX;
  y=e.clientY;
  updateScreen();
}

function updateScreen() {
  var el = document.getElementById('mega');
  el.style.left=rx+"px";
  el.style.top=ry+"px";
}

