// eda_graph.js -- Apr 23, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

//*****************************************************************************
//***********************************DRAWING***********************************
//*****************************************************************************

//place the codeboxes and draw the lines

var nodes;  //so I can get debug data on it

function graphDraw(data, divlist)
{
  //get all the boxes on the field
  nodes = new Array();
  var lines = new Array();  //memory is cheap
  var segments = new Array(); //segments[end]=start;

  firstaddr=-1;

  for(a=0;a<divlist.length;a++) {
    if(divlist[a].nodeType==1 && divlist[a].childNodes.length>3)  {
      //alert(divlist[a].nodeName);
      //alert(divlist[a].childNodes[3].className);
      //alert(divlist[a].childNodes[2].id);

      //alert("got node at: "+divlist[a].childNodes[3].id);
      if(firstaddr==-1) firstaddr=divlist[a].childNodes[3].id;
      nodes[divlist[a].childNodes[3].id]={
        box: divlist[a],
        level: null,
        parents: new Array(),   //from parents
        children: new Array(),  //to children
        lineage: new Array(),
        localloop: false,     //links to itself
        xwidth: 0,
        xcenter: 0,
        xweight: 0};
      segments[divlist[a].lastChild.previousSibling.id]=divlist[a].childNodes[3].id;
    }
  }

  for(a=0;a<data.length;a++) {
    m=data[a].attributes;
    if(m) {
      //[0] is color, [1] is to, [2] is from
      if(nodes[segments[m[2].value]]==null) alert(m[2].value + "  "+segments[m[2].value]);
      nodes[segments[m[2].value]].children.push(m[1].value);
      if(nodes[m[1].value]==null) alert(m[1].value);
      nodes[m[1].value].parents.push(segments[m[2].value]);

      lines.push({to: m[1].value, from: segments[m[2].value], color: m[0].value});
    }
  }
//do initial traverse, build edges
  graphRemoveLoops(nodes,firstaddr);

//place all childless nodes in bottom level
  var inLevel = new Array();
  for(a in nodes) {
    if(nodes[a].children.length==0) {
      nodes[a].level=0;
      inLevel.push(a);
    }
  }
//do Y placement

//should insert fake nodes for routing
  doYPlacement(nodes, 0,inLevel);

//create level array
  var level = new Array();

  for(a in nodes) {
    //alert(nodes[a].level);
    if(level[nodes[a].level]==null) {
      /*level[nodes[a].level]=document.createElement("div");
      level[nodes[a].level].className="level";
      level[nodes[a].level].id="L_"+nodes[a].level;*/
      level[nodes[a].level]=new Array();
    }
    //level[nodes[a].level].appendChild(nodes[a].box);
    level[nodes[a].level].push(a);
  }
  /*if(level[null]!=null)
  {
    level[null].style.backgroundColor="red";
    field.appendChild(level[null]);
  }*/

//sort levels, ghetto crossing minimization
  var levelsSorted = new Array();
  for(a=0;a<level.length;a++) {
    //geohot.debug("L"+a+": "+level[a]);
    if(level[a]==null) {
      geohot.debug("level "+a+" doesn't exist");
      break;
    }
    levelsSorted[a]=level[a].sort();
  }

  //alert("sorted: "+levelsSorted.length);


//clear the field
  field=document.getElementById("mega");
  field.innerHTML="<canvas id=\"line_canvas\" width=\"3000px\" height=\"3000px\"></canvas>";
//draw the levels
  //levels start minus
  var levelDiv = new Array();
  for(a=(levelsSorted.length-1);a>=0;a--) {
    if(levelsSorted[a]==null) alert("level "+a+" not found");
    else {
      levelDiv[a]=document.createElement("div");
      levelDiv[a].className="level";
      levelDiv[a].id="L_"+a;
      for(n=0;n<levelsSorted[a].length;n++) {
        //alert(levelsSorted[a][n]);
        levelDiv[a].appendChild(nodes[levelsSorted[a][n]].box);
      }
      field.appendChild(levelDiv[a]);
    }
  }

//X placement here

//populate node xwidths
  for(a in nodes) {
    nodes[a].xwidth=nodes[a].box.offsetWidth; //since boxes are placed
  }
//place bottom node(s)
  for(a in levelsSorted[0]) {
    nodes[levelsSorted[0][a]].xcenter=500;
    nodes[levelsSorted[0][a]].xweight=1;
  }

//place higher up nodes
//for each level
  for(a=1;a<level.length;a++) {
    //for each node in that level
    for(b=0;b<levelsSorted[a].length;b++) {
      //get average of all children of b placements
      var averageXCenter=0, averageXCount=0;
      for(c in nodes[levelsSorted[a][b]].children) {
        averageXCenter+=nodes[nodes[levelsSorted[a][b]].children[c]].xcenter;
        averageXCount++;
      }
      averageXCenter/=averageXCount;

      //alert("node "+levelsSorted[a][b]+" wants to be placed at "+averageXCenter);

      //my ideal placement is averageXCenter
      nodes[levelsSorted[a][b]].xcenter=averageXCenter;

      //but others have a say too
      //alert("a,b "+a+" "+b);

      for(c=(b-1);c>=0;c--) {
        //if(c==-1) break;
        //alert("c" + c);
        //if left edge less than last right edge
        var leftedge_this=(nodes[levelsSorted[a][c+1]].xcenter-((nodes[levelsSorted[a][c+1]].xwidth)/2));
        var rightedge_last=(nodes[levelsSorted[a][c]].xcenter+((nodes[levelsSorted[a][c]].xwidth)/2));
        //alert(leftedge_this+" "+rightedge_last);
        if( leftedge_this < rightedge_last )
        {
          nodes[levelsSorted[a][c+1]].xcenter+=(rightedge_last-leftedge_this)+20;
        }
      }
    }

//    alert(" ");
  }

  graphRenderX(nodes, levelsSorted);



//draw the lines
  for(a in lines) {
    routeLine(
      ((nodes[lines[a].to].box.offsetLeft)+(nodes[lines[a].to].box.offsetWidth/2)),
      nodes[lines[a].to].box.offsetTop,
      ((nodes[lines[a].from].box.offsetLeft)+(nodes[lines[a].from].box.offsetWidth/2)),
      (nodes[lines[a].from].box.offsetTop+nodes[lines[a].from].box.offsetHeight),
      lines[a].color);
  }
}

function graphRenderX(nodes, levelsSorted) {
//apply xcenters on field, hack uses marginLeft
  for(a=0;a<levelsSorted.length;a++) {
    for(b=0;b<levelsSorted[a].length;b++) {
      var calcpad;
      if(b==0)
        calcpad=nodes[levelsSorted[a][b]].xcenter-((nodes[levelsSorted[a][b]].xwidth)/2);
      else
      {
        //alert(nodes[levelsSorted[a][b-1]].box.offsetLeft+nodes[levelsSorted[a][b-1]].box.offsetWidth);
        calcpad=nodes[levelsSorted[a][b]].xcenter-((nodes[levelsSorted[a][b]].xwidth)/2)-(nodes[levelsSorted[a][b-1]].box.offsetLeft+nodes[levelsSorted[a][b-1]].box.offsetWidth);
      }
      nodes[levelsSorted[a][b]].box.style.marginLeft=calcpad+"px";
    }
  }
}


function routeLine(sx, sy, ex, ey, c) {  //start has the arrow
  var line=new Array();
  line.push({x: sx, y: sy});
  line.push({x: sx, y: (sy+ey)/2}); //come up
  line.push({x: ex, y: (sy+ey)/2}); //come over
  line.push({x: ex, y: ey});  //and up
  //alert(sx + ", " +sy + " - " + ex + ", " + ey);
  drawlinearray(line, c);
}

function graphRemoveLoops(nodes, thisnodeindex) {
  //iterate through all children
  for(a in nodes[thisnodeindex].children)
  {
    var runChild=true;
    for(l in nodes[thisnodeindex].lineage) {
      //search my lineage for this children
      if(nodes[thisnodeindex].lineage[l]==nodes[thisnodeindex].children[a]) {
        runChild=false;
        break;
      }
    }
    if(runChild==false) {
      //my child was found in my lineage, we have a problem

      var it=nodes[nodes[thisnodeindex].children[a]];
      var it_nodeindex=nodes[thisnodeindex].children[a];

      for(c in it.parents) {
        //i'm not it's parent anymore
        if(it.parents[c] == thisnodeindex) {
          it.parents.splice(c,1);
          break;
        }
      }
      nodes[thisnodeindex].children.splice(a,1);  //and it's not my child anymore

      if(thisnodeindex!=it_nodeindex) {
        //insert the other way
        nodes[thisnodeindex].parents.splice(0,0,it_nodeindex); //it's actually my parent
        it.children.splice(0,0,thisnodeindex);          //and i am it's child
        //geohot.debug("found mixed up line");
      }
      else {    //not even sure we need to track these
        //geohot.debug("found local loop");
        nodes[thisnodeindex].localloop=true;
      }

    }
    else {
      nodes[nodes[thisnodeindex].children[a]].lineage=nodes[thisnodeindex].lineage.concat(
        nodes[nodes[thisnodeindex].children[a]].lineage, thisnodeindex);
      graphRemoveLoops(nodes, nodes[thisnodeindex].children[a]);
    }
  }
}

function doYPlacement(nodes,level,inLevel) {
  //geohot.debug(level+": "+inLevel);
//i guess it's not really recursive
  if(level>50)
  {
    geohot.debug("too many levels");
    return;
  }
//  alert("placing level: "+level);
//build set of node indexes in level
  var inLevelnext = new Array();
//for all nodes in this level
  for(a in inLevel) {
    //place all nodes parents in the next level
    for(p in nodes[inLevel[a]].parents) {
      /*geohot.debug("placing " +nodes[inLevel[a]].parents[p]+" in level "+(level+1)+
        " because of "+inLevel[a]+ " with lineage "+nodes[inLevel[a]].lineage);*/
      nodes[nodes[inLevel[a]].parents[p]].level=level+1;
      inLevelnext.push(nodes[inLevel[a]].parents[p]);
    }
  }
//remove dupes in inLevelnext
  for(a=0;a<inLevelnext.length;a++)
  {
    for(b=(a+1);b<inLevelnext.length;b++)
    {
      if(inLevelnext[a]==inLevelnext[b])
        inLevelnext.splice(b,1);
    }
  }

  if(inLevelnext.length>0)
    doYPlacement(nodes, level+1, inLevelnext);
}


/*function doYPlacement(nodes,levels)
//function is stupid for now
{
  for(a=0;(a<levels.length&&a<10);a++)
  {
    for(nodename in levels[a])
    {
      thisnode=nodes[levels[a][nodename]];
      //place all children in the level below this one
      for(child in thisnode.children)
      {
        //if(nodes[thisnode.children[child]].level==null) //unplaced
        //{
          if(levels[a+1]==null) levels[a+1]=new Array();
          nodes[thisnode.children[child]].level=a+1;
          levels[a+1].push(thisnode.children[child]);
        //}
      }
    }
  }
  return 0;
}*/
