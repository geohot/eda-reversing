// eda_graph.js -- Apr 23, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

//*****************************************************************************
//***********************************DRAWING***********************************
//*****************************************************************************

//place the codeboxes and draw the lines

var nodes=[];  //so I can get debug data on it
var level;

var tt = new Object;

window.addEventListener("load", function() {
  geohot.debug("hello");
  tt["400"]=1;
  console.debug(tt.length);
}, false);

function graphDraw(data, divlist)
{
  //get all the boxes on the field
  var lines = Array();  //memory is cheap
  var segments = Array(); //segments[end]=start;

  firstaddr=-1;

  geohot.debug(nodes.length);

  for(a=0;a<divlist.length;a++) {
    //alert(divlist[a].nodeName);
    //alert(divlist[a].childNodes[3].className);
    //alert(divlist[a].childNodes[2].id);

    //alert("got node at: "+divlist[a].childNodes[3].id);
    if(firstaddr==-1) firstaddr=divlist[a].childNodes[1].id;
    nodes[divlist[a].childNodes[1].id]={
      box: divlist[a],
      level: null,
      parents: new Array(),   //from parents
      children: new Array(),  //to children
      lineage: new Array(),
      localloop: false,     //links to itself
      xwidth: 0,
      xcenter: 0,
      xweight: 0};
    segments[divlist[a].lastChild.id]=divlist[a].childNodes[1].id;
  }

  geohot.debug(nodes.length);

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

  geohot.debug(nodes.length);

//place all childless nodes in bottom level
  var inLevel = new Array();
  for(a in nodes) {
    if(nodes[a].children.length==0) {
      nodes[a].level=0;
      inLevel.push(a);
    }
  }
//do Y placement
  doYPlacement(nodes, 0,inLevel);


//create level array
  level = new Array();

  for(a in nodes) {
    if(level[nodes[a].level]==null) {
      level[nodes[a].level]=new Array();
    }
    level[nodes[a].level].push(a);
  }

//sort bottom level for ordering
  level[0]=level[0].sort();

  geohot.debug(nodes.length);

//insert fake nodes
  for(a=0;a<level.length;a++) {
    for(n in level[a]) {
      var parray=nodes[level[a][n]].parents;
      for(c in parray) {  //will be 0 for the top level
        if(nodes[parray[c]].level!=(a+1)) {   //if not on the level directly above thiss
          var newname=level[a][n]+"-"+parray[c];
          nodes[newname] = {
            box: null,
            parents: new Array(parray[c]),
            children: new Array(level[a][n]),
            level: a+1};

          level[a+1].push(newname);
          //geohot.debug(parray);
          for(n in nodes[parray[c]].children) {    //remove the child and insert this
            //geohot.debug("node "+nodes[parray[c]].children[n]+" search for "+ level[a][n]);
            if(nodes[parray[c]].children[n]==level[a][n]) {
              //geohot.debug("removed");
              nodes[parray[c]].children.splice(n, 1, newname);
              break;
            }
          }
          parray.splice(c, 1, newname); //not a direct parent anymore, removed from that loc and added link
          //geohot.debug(parray);
        }
      }
    }
  }

  //alert("sorted: "+level.length);


//clear the field
  field=document.getElementById("mega");
  field.innerHTML="<canvas id=\"line_canvas\" width=\"3000px\" height=\"3000px\"></canvas>";
//draw the levels
  //levels start minus
  var levelDiv = new Array();
  for(a=(level.length-1);a>=0;a--) {
    if(level[a]==null) alert("level "+a+" not found");
    else {
      levelDiv[a]=document.createElement("div");
      levelDiv[a].className="level";
      levelDiv[a].id="L_"+a;
      for(n=0;n<level[a].length;n++) {
        //alert(level[a][n]);
        if(nodes[level[a][n]].box != null)
          levelDiv[a].appendChild(nodes[level[a][n]].box);
        else {  //for testing
          nodes[level[a][n]].box = document.createElement("div");
          nodes[level[a][n]].box.className="codebox";
          nodes[level[a][n]].box.innerHTML=level[a][n];
          //nodes[level[a][n]].box.innerHTML="X";
          levelDiv[a].appendChild(nodes[level[a][n]].box);
        }
      }
      field.appendChild(levelDiv[a]);
    }
  }

//X placement here

//populate node xwidths
  for(a in nodes) {
    if(nodes[a].box==null)
      nodes[a].xwidth=0;
    else
      nodes[a].xwidth=nodes[a].box.offsetWidth; //since boxes are placed
  }
//place bottom node(s)
  for(a in level[0]) {
    nodes[level[0][a]].xcenter=500;
    nodes[level[0][a]].xweight=1;
    for(c=(a-1);c>=0;c--) {
      //if(c==-1) break;
      //alert("c" + c);
      //if left edge less than last right edge
      var leftedge_this=(nodes[level[0][c+1]].xcenter-((nodes[level[0][c+1]].xwidth)/2));
      var rightedge_last=(nodes[level[0][c]].xcenter+((nodes[level[0][c]].xwidth)/2));
      //alert(leftedge_this+" "+rightedge_last);
      if( leftedge_this < rightedge_last )
      {
        nodes[level[0][c+1]].xcenter+=(rightedge_last-leftedge_this)+20;
      }
    }
  }

//place higher up nodes
//for each level
  for(a=1;a<level.length;a++) {
    //for each node in that level
    for(b=0;b<level[a].length;b++) {
      //get average of all children of b placements
      var averageXCenter=0, averageXCount=0;
      for(c in nodes[level[a][b]].children) {
        averageXCenter+=nodes[nodes[level[a][b]].children[c]].xcenter;
        averageXCount++;
      }
      averageXCenter/=averageXCount;

      //alert("node "+level[a][b]+" wants to be placed at "+averageXCenter);

      //my ideal placement is averageXCenter
      nodes[level[a][b]].xcenter=averageXCenter;

      //but others have a say too
      //alert("a,b "+a+" "+b);

/*      for(c=(b-1);c>=0;c--) {
        //if(c==-1) break;
        //alert("c" + c);
        //if left edge less than last right edge
        var leftedge_this=(nodes[level[a][c+1]].xcenter-((nodes[level[a][c+1]].xwidth)/2));
        var rightedge_this=(nodes[level[a][c+1]].xcenter+((nodes[level[a][c+1]].xwidth)/2));
        var leftedge_last=(nodes[level[a][c]].xcenter-((nodes[level[a][c]].xwidth)/2));
        var rightedge_last=(nodes[level[a][c]].xcenter+((nodes[level[a][c]].xwidth)/2));

        //alert(leftedge_this+" "+rightedge_last);
        if( ((leftedge_this<leftedge_last) && (leftedge_last<rightedge_this)) ||
            ((leftedge_this<rightedge_last) && (rightedge_last<rightedge_this)) )
        {
          //nodes[level[a][c+1]].xcenter+=(rightedge_last-leftedge_this)+20;
          nodes[level[a][c+1]].xcenter+=((rightedge_last-leftedge_this)+20);
          //nodes[level[a][c]].xcenter-=((rightedge_last-leftedge_this)+20)/2;
        }
      }*/
    }

    //alert(" ");
  }


  geohot.debug(nodes.length);

  graphRenderX(nodes, level);




//draw the lines
  /*for(a in lines) {
    routeLine(
      ((nodes[lines[a].to].box.offsetLeft)+(nodes[lines[a].to].box.offsetWidth/2)),
      nodes[lines[a].to].box.offsetTop,
      ((nodes[lines[a].from].box.offsetLeft)+(nodes[lines[a].from].box.offsetWidth/2)),
      (nodes[lines[a].from].box.offsetTop+nodes[lines[a].from].box.offsetHeight),
      lines[a].color);
  }*/
  for(a=0;a<level.length;a++) {
    //for each node in that level
    for(b=0;b<level[a].length;b++) {
      xoffset=0;
      for(c in nodes[level[a][b]].parents) {
        //xoffset+=10;
        routeLine(nodes[level[a][b]].xcenter+xoffset,
                   nodes[level[a][b]].box.offsetTop,
                   nodes[nodes[level[a][b]].parents[c]].xcenter,
                   nodes[nodes[level[a][b]].parents[c]].box.offsetTop+
                    nodes[nodes[level[a][b]].parents[c]].box.offsetHeight,
                   "red");
      }
    }
  }
}

function graphRenderX(nodes, level) {
//apply xcenters on field, hack uses marginLeft
  for(a=0;a<level.length;a++) {
    for(b=0;b<level[a].length;b++) {
      var calcpad;
      if(b==0)
        calcpad=nodes[level[a][b]].xcenter-((nodes[level[a][b]].xwidth)/2);
      else {
        //alert(nodes[level[a][b-1]].box.offsetLeft+nodes[level[a][b-1]].box.offsetWidth);
        //left edge of current - right edge of last
        calcpad=(nodes[level[a][b]].xcenter-((nodes[level[a][b]].xwidth)/2))
          -(nodes[level[a][b-1]].xcenter+(nodes[level[a][b-1]].xwidth/2));
        //calcpad=0;
      }
      if ( nodes[level[a][b]].box != null ) {
        nodes[level[a][b]].box.style.marginLeft=calcpad+"px";
      }
      //nodes[level[a][b]].box.style.left=nodes[level[a][b]].xcenter-((nodes[level[a][b]].xwidth)/2) + "px";
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


//this is step 1 of the sugiyama method, and it works
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
      
      geohot.debug("switching "+it_nodeindex+" and "+thisnodeindex);

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

window.addEventListener("click", function(e) {
  //geohot.debug(e.target.parentNode.id + " "+ e.target.classAn);
  if(nodes[e.target.parentNode.id]!=null) {
    geohot.debug(nodes[e.target.parentNode.id]);
  }
},false);

//this is step two of the sugiyama method and it works too
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
