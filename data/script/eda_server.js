// eda_server.js -- Apr 23, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

//*****************************************************************************
//****************************SERVER COMMUNICATION*****************************
//*****************************************************************************


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
    gx=(-onPage.offsetLeft)+500;
    gy=(-onPage.offsetTop)+500;
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