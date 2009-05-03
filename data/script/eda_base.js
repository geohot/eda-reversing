// eda_base.js -- Apr 23, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

//*****************************************************************************
//**********************************STARTUP************************************
//*****************************************************************************

var geohot;   //the debug symbol all over

if(console!=null) geohot=console;
else geohot.debug=function(e){}

function include(fn) {
  document.write('<script type="text/javascript" src="'+fn+'"></scr'+'ipt>');
}

include("script/eda_events.js");   //EDA events, like renaming
include("script/eda_server.js");   //communication with EDA server
include("script/eda_graph.js");    //graph drawing algorithm
include("script/eda_drawing.js");  //drawing routines

window.addEventListener('load', function(e) {
  //hacky, i know, find a better way to do this
  if(document.title=="EDA")
    initFunctionViewer();
  else if(document.title=="EDA Bank Viewer")
    initBankViewer();
}, false);

function initFunctionViewer() {
  geohot.debug("hello, I'm EDA");

  initEvents();
  refreshFunctionList();

  if(location.hash!=null&&location.hash!="") {    //for refresh
    refreshFunction(location.hash.replace("#",""));
  }
}

function initBankViewer() {
  geohot.debug("hello, I'm EDA Bank Viewer");
  document.getElementById("address").onchange=getHexData;
  document.getElementById("clnum").onchange=getHexData;
  getHexData();
}










