//*****************************************************************************
//**********************************STARTUP************************************
//*****************************************************************************

window.addEventListener('load', function(e) {
  window.addEventListener("mousewheel",mouseWheelHandler,true);
  mode=1;
  rx=0;
  ry=0;
  moving=0;
  document.getElementById('mega').onmousedown=function(){return false};
  document.getElementById('functionlist').onmousedown=function(){return false};

  refreshFunctionList();
  //refreshFunction("4010ec");
}, false);

//*****************************************************************************
//*******************************KEYBOARD**************************************
//*****************************************************************************

var mode;  //0 is text, 1 is graph

function capturekey(e){
  //alert(e.keyCode);
  if(e.keyCode==32)
  {
    var el = document.getElementById('mega');
    if(mode==0)  //switch to graph
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
  }
}

//*****************************************************************************
//****************************SERVER COMMUNICATION*****************************
//*****************************************************************************


function refreshFunction(address)
//pull the function instructions+branch tree down from the server
{
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
  drawGraph(reqBD.responseXML.documentElement.childNodes, address, bigdiv.childNodes);
//******Cleanup******
  }
  }


}

function refreshFunctionList()
// pull the function list from the server
{
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
        "'"+m[a].attributes[0].value+"'"+')">'+
        m[a].childNodes[0].nodeValue+'</div>';
    }
  }
  box.innerHTML=spit;
//******Cleanup******
  }
  }
}

//*****************************************************************************
//*******************************SCREEN MOVEMENT*******************************
//*****************************************************************************

var x,y;
var rx,ry;
var moving;

function mouseWheelHandler(e){
  //alert("Scroll");
  var el = document.getElementById('mega');
  ry=(ry+e.wheelDelta/120);
  el.style.top=ry+"px";
}

function movescreen(e) {
  var el = document.getElementById('mega');
  rx=rx+(e.clientX-x);
  ry=ry+(e.clientY-y);
  x=e.clientX;
  y=e.clientY;
  el.style.left=rx+"px";
  el.style.top=ry+"px";
}

window.addEventListener('mousedown', function(e) {

  if(e.target.id=="root" || 
    e.target.parentNode.getAttribute("class")=="frame" ||
    e.target.getAttribute("class")=="bg")
  {
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
    if(moving!=1)
    {
      //highlight(e);
      //deselectAll();
    }
    moving=0;
  }
},false);


//*****************************************************************************
//***********************************DRAWING***********************************
//*****************************************************************************

//place the codeboxes and draw the lines
function drawGraph(data, topBox, divlist)
{
  //get all the boxes on the field
  nodes = new Array();
  levels = new Array();
  lines = new Array();  //memory is cheap
  segments = new Array(); //segments[end]=start;

  for(a=0;a<divlist.length;a++) {
    if(divlist[a].nodeType==1)
    {
      nodes[divlist[a].childNodes[1].id]={ 
        box: divlist[a],
        level: null, 
        parents: new Array(),   //from parents
        parenttypes: new Array(),
        children: new Array(),  //to children
        lineage: new Array(),
        widthx: 0, 
        centerx: 0,
        xweight: 0};
      segments[divlist[a].lastChild.previousSibling.id]=divlist[a].childNodes[1].id;
    }
  }

  nodes[topBox].level=0;
  levels[0]=new Array();
  levels[0].push(topBox);

  for(a=0;a<data.length;a++)
  {
    m=data[a].attributes;
    if(m)
    {
      //[0] is color, [1] is to, [2] is from
      //node[segments[m[2].value]].box.style.backgroundColor="yellow";
      nodes[segments[m[2].value]].children.push(m[1].value);
      nodes[m[1].value].parents.push(segments[m[2].value]);
      lines.push({to: m[1].value, from: segments[m[2].value], color: m[0].value});
    }
  }

//do Y placement
  doYPlacement(nodes,levels);
//X placement is implied for now

//clear the field
  field=document.getElementById("mega");
  field.innerHTML="<canvas id=\"line_canvas\" width=\"3000px\" height=\"3000px\"></canvas>";
//place the div boxes
  for(a=0;a<levels.length;a++)
  {
    thislevel=document.createElement("div");
    thislevel.className="level";
    for(nodename in levels[a])
    {
      thislevel.appendChild(nodes[levels[a][nodename]].box);
    }
    field.appendChild(thislevel);
  }

//draw the lines
  /*for(a in nodes) {
    nodes[a].widthx=nodes[a].box.offsetWidth;
    nodes[a].centerx=nodes[a].box.offsetLeft+(nodes[a].box.offsetWidth)/2;
    drawline(nodes[a].centerx, nodes[a].box.offsetTop, 0, -200);
  }*/
  for(a in lines)
  {
    routeLine(
      ((nodes[lines[a].to].box.offsetLeft)+(nodes[lines[a].to].box.offsetWidth/2)),
      nodes[lines[a].to].box.offsetTop,
      ((nodes[lines[a].from].box.offsetLeft)+(nodes[lines[a].from].box.offsetWidth/2)),
      (nodes[lines[a].from].box.offsetTop+nodes[lines[a].from].box.offsetHeight),
      lines[a].color);
  }
}

function routeLine(sx, sy, ex, ey, c)  //start has the arrow
{
  var line=new Array();
  line.push({x: sx, y: sy});
  line.push({x: sx, y: (sy+ey)/2}); //come up
  line.push({x: ex, y: (sy+ey)/2}); //come over
  line.push({x: ex, y: ey});  //and up
  //alert(sx + ", " +sy + " - " + ex + ", " + ey);
  drawlinearray(line, c);
}


function doYPlacement(nodes,levels)
//function is stupid for now
{
  for(a=0;a<levels.length;a++)
  {
    for(nodename in levels[a])
    {
      thisnode=nodes[levels[a][nodename]];
      for(child in thisnode.children)
      {
        if(nodes[thisnode.children[child]].level==null) //unplaced
        {
          if(levels[a+1]==null) levels[a+1]=new Array();
          nodes[thisnode.children[child]].level=a+1;
          levels[a+1].push(thisnode.children[child]);
        }
      }
    }
  }
  return 0;
}

//*****************************************************************************
//***************************LOW LEVEL DRAWING*********************************
//*****************************************************************************

function drawarrow(x,y,c)
{
	ctx=document.getElementById('line_canvas').getContext('2d');
	ctx.beginPath();
	if(c=="red")
		ctx.fillStyle='rgba(255,0,0,255)';
	else if(c=="green")
		ctx.fillStyle='rgba(0,128,0,255)';
	else if(c=="blue")
		ctx.fillStyle='rgba(0,0,255,255)';
	ctx.moveTo(x,y);
	ctx.lineTo(x-5, y-10);
	ctx.lineTo(x+5, y-10);
	ctx.lineTo(x,y);
	ctx.fill();
}

function drawline(x,y,w,h,c)
{
	animatedctx=document.getElementById('line_canvas').getContext('2d');
	animatedctx.beginPath();
	//animatedctx.fillStyle=rgba(255,255,255,0);
	if(c==0)
		animatedctx.fillStyle='rgba(255,0,0,128)';
	else if(c==1)
		animatedctx.fillStyle='rgba(0,128,0,128)';
	else if(c==2)
		animatedctx.fillStyle='rgba(0,0,255,128)';
	if(w<0)
	{
		x=x+w;		//move x left
		w=-w;
	}
	else
		w++;
	if(h<0)
	{
		y=y+h;		//move y up
		h=-h;
	}
	else
		h++;

	animatedctx.rect(x,y,w,h);
	animatedctx.fill();
}

function drawlinearray(a,c)		//a is {x,y}, c is the color
{
	//this should draw an arrow too
	drawarrow(a[0].x,a[0].y,c);

	for(var i=1;i<a.length;i++)
	{
		if(a[i].x==a[i-1].x)		//x is the same
			drawline(a[i-1].x, a[i-1].y, 0, a[i].y-a[i-1].y,c);

		if(a[i].y==a[i-1].y)		//y is the same
			drawline(a[i-1].x, a[i-1].y, a[i].x-a[i-1].x, 0,c);
	}
}
