// eda_server.js -- Apr 23, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/


//*****************************************************************************
//****************************HEX DUMP SUPPORT*********************************
//*****************************************************************************

//shouldn't be in server, i'm lazy for now
var hexStartAddr;
var hexArray=Array();
var hexEditing=Array();
var selecting=false;
function startHexDumpSelect(e) {
  if(e.target.class=="hexcell") {
    for(b in hexEditing)
      hexArray[hexEditing[b]].style.backgroundColor="";
    hexEditing.length=0;
    hexEditing.push(parseInt(e.target.id,16)-hexStartAddr);
    //geohot.debug(hexEditing[0]);
    hexArray[hexEditing[0]].style.backgroundColor="orange";
    selecting=true;
    e.preventDefault();
    return false;
  }
  else {
    for(b in hexEditing)
      hexArray[hexEditing[b]].style.backgroundColor="";
    hexEditing.length=0;
  }
}

var olden;

window.addEventListener("keypress", function(e) {
  //geohot.debug(e);
  if(hexEditing.length>0) {
    if( (e.charCode>=48 && e.charCode<58) || (e.charCode>=97 && e.charCode<103)) {
      hexArray[hexEditing[0]].style.backgroundColor="red";
      if(hexArray[hexEditing[0]].innerHTML.length==2)
        hexArray[hexEditing[0]].innerHTML="";
      hexArray[hexEditing[0]].innerHTML+=String.fromCharCode(e.charCode).toUpperCase();
    }
    if(hexArray[hexEditing[0]].innerHTML.length==2) {
      hexArray[hexEditing[0]].style.backgroundColor="orange";
    }
    if(e.charCode==13) {  //send command
    }
    //if(hexEditing.length==1) {
    if(e.keyCode==37) {
      hexArray[hexEditing[0]].style.backgroundColor="";
      hexEditing[0]--;
      geohot.debug("left");
      hexArray[hexEditing[0]].style.backgroundColor="orange";
    }
    else if(e.keyCode==39) {
      hexArray[hexEditing[0]].style.backgroundColor="";
      geohot.debug("right");
      hexEditing[0]++;
      hexArray[hexEditing[0]].style.backgroundColor="orange";
    }
    geohot.debug(e.keyCode);
    //}
  }
},false);

function moveHexDumpSelect(e) {
  if(selecting==true) {
    if(e.target.class!="hexcell") {
      selecting=false;
    }
    else {
      var st=hexEditing[0];
      var en=parseInt(e.target.id,16)-hexStartAddr;
      if(olden!=en) {
        //geohot.debug(e.target.nodeName);
        for(b in hexEditing) {
          //geohot.debug(b, hexEditing[b]);
          hexArray[hexEditing[b]].style.backgroundColor="";
        }
        hexEditing.length=1;
        //geohot.debug("selected: ", (en-st));
        while( st<en || st>en ) {
          //geohot.debug(st);
          hexArray[st].style.backgroundColor="orange";
          hexEditing.push(st);
          if(st<en)
            st+=1;
          else
            st-=1;
        }
        hexArray[st].style.backgroundColor="orange";
        hexEditing.push(st);
        olden=en;
      }
    }

    e.preventDefault();
    return false;
  }
}

function endHexDumpSelect(e) {
  selecting=false;
  e.preventDefault();
  return false;
}

window.addEventListener("mousedown",startHexDumpSelect,false);
window.addEventListener("mousemove",moveHexDumpSelect,false);
window.addEventListener("mouseup",endHexDumpSelect,false);



//*****************************************************************************
//****************************SERVER COMMUNICATION*****************************
//*****************************************************************************

//fetch hexdump data from the Bank
function getHexData() {
  var addr=document.getElementById("address").value;
  var clnum=document.getElementById("clnum").value;
  if(addr=="" || clnum=="") return false;

  hexStartAddr=parseInt(addr, 16);

  var req = new XMLHttpRequest();
  req.open("GET", "Bank/getMemory/"+addr+"/200/"+clnum, true); req.send("");

  req.onreadystatechange = function(){
    if(req.readyState==4) {
      //raw memory returned as html...for now
      if(document.getElementById("hexdump")!=null)
        document.body.removeChild(document.getElementById("hexdump"));
      hexdump=document.createElement("hex");
      hexdump.id="hexdump";
      hexdump.innerHTML="";
      a=0;
      currtr=document.createElement("tr");
      while(a<req.responseText.length) {
        if(a!=0 && (a%64)==0) {
          hexdump.appendChild(currtr);
          currtr=document.createElement("tr");
        }
        if((a%32)==0) {
          currtd=document.createElement("td");
          currtd.style.paddingRight="10px";
          currtr.appendChild(currtd);
        }
        currtd=document.createElement("td");
        currtd.id=(parseInt(addr,16)+(a/2)).toString(16);
        currtd.class="hexcell";
        currtd.style.border="1px solid gray";
        //currtd.addEventListener("mousedown",startHexDumpSelect,false);
        //currtd.addEventListener("mousemove",moveHexDumpSelect,false);
        //currtd.addEventListener("mouseup",endHexDumpSelect,false);
        //currtd.onclick=editHexData;
        currtd.innerHTML=req.responseText.substr(a,2).toUpperCase();
        hexArray.push(currtd);
        currtr.appendChild(currtd);
        a+=2;
      }
      hexdump.appendChild(currtr);
      document.body.appendChild(hexdump);
    }
  };


}

//send a rename request to the server
function sendRename(a,b) {
  var req = new XMLHttpRequest();
  req.open("GET", "Bank/rename/"+a+"/"+b, true); req.send("");
}

//pull the function instructions+branch tree down from the server
function refreshFunction(address) {
  //alert("refreshfunction "+address);

  onPage=document.getElementById(address);
  if(onPage!=null)
  {
    //move the screen

    //alert("found on page");
    gx=(-onPage.offsetLeft)+300;
    gy=(-onPage.offsetTop)+300;
    glideInterval=setInterval("glide()",20);
    return;
  }

  selected=null;  //selections no longer valid, this is a real update

  var req = new XMLHttpRequest(), reqBD = new XMLHttpRequest();
  req.open("GET", "Bank/getFunction/"+address, true); req.send("");

  req.onreadystatechange = function(){
  if (req.readyState == 4) {
//*****Got Response*****

//nested to allow drag everywhere
  bigdiv=document.createElement("div");
  bigdiv.innerHTML=req.responseText;
  //alert(bigdiv.innerHTML);

  //document.getElementById("mega").appendChild(field);

//now request the branch data to draw the graph
  reqBD.open("GET", "Bank/getFunctionBranchData/"+address, true); reqBD.send("");
//******Cleanup******
  }
  }

  reqBD.onreadystatechange = function(){
  if (reqBD.readyState == 4) {
//*****Got Response*****
  graphDraw(reqBD.responseXML.documentElement.childNodes, bigdiv.childNodes);
  
  onPage=document.getElementById(address);
  if(onPage!=null) {
    rx=(-onPage.offsetLeft)+500;
    ry=(-onPage.offsetTop)+100;
    updateScreen();
  }
  
//******Cleanup******
  }
  }

  oldhash="#"+address;
  location.hash=address;
}

// pull the function list from the server
function refreshFunctionList() {
  req = new XMLHttpRequest()
  req.open("GET", "Bank/getFunctionList", true);
  req.send("");
  req.onreadystatechange = function(){
  if (req.readyState == 4) {
//*****Got Response*****
  var m=req.responseXML.documentElement.childNodes;
  var box=document.getElementById("functionlist");
  spit="";
  for(a=0;a<m.length;a++)
  {
    if(m[a].attributes)
    {
      //alert(m[a].childNodes[0].nodeValue);
      spit+='<div class="function" ondblclick="refreshFunction('+
        //"'"+m[a].attributes[0].value+"'"+')">'+
        "'"+m[a].childNodes[0].nodeValue+"'"+')">'+
        m[a].childNodes[0].nodeValue+'</div>';
    }
  }
  box.innerHTML=spit;
//******Cleanup******
  }
  }
}