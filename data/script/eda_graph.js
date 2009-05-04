// eda_graph.js -- May 3, 2009
//   by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

// this is the third EDA graph drawer, and this one will be good
// 1. learn javascript
// 2. write sugiyama graph layout
// 3. ???
// 4. profit

// yea i'm doing work at work!!!
// okay so heres the design
// Node is an object with functions, smart functions
// and like prototypes and shit

// OMG i thought i lost this
// ext3 is scary

// graph is a global object for everybody
var graph = {};

// helper functions

// see if theres overlap in two arrays
function arrayOverlap(a1, a2) {
  for(var i=0; i<a1.length;i++) {
    for(var j=i; j<a2.length; j++) {
      if(a1[i] == a2[j])
        return true;
    }
  }
  return false;
}

function arrayMakeUnique(a1) {
  for(var i=0; i<a1.length;i++) {
    for(var j=i+1; j<a1.length; j++) {
      if(a1[i] == a1[j])
        a1.splice(j, 1);
    }
  }
}

function stringNodeArray(na) {
  var pr = "[";
  for(var a=0; a<na.length; a++) {
    pr+=na[a].address+" ";
  }
  if (pr.length>1)
    pr = pr.substr(0, pr.length-1);
  pr+="]";
  return pr;
}

// Node class
function Node(divbox, sa, ea) {
  //geohot.debug(divbox.childNodes);
  // constructor for a Node
  /*this.parents = [];
  this.children = [];*/
  this.lineage = [];
  this.level = null;
  this.xwidth = 200;   //this is the default width for x ordering
  this.xcenter = null; //this is the default center
  this.yheight = 0;
  this.box = divbox;
  this.address = sa;
  this.endaddress = ea;
}

function NodeFromDiv(divbox) {
  var address, endaddress;

  for(a in divbox.childNodes) {
    // find first instruction and get the address
    if (divbox.childNodes[a].className == "instruction") {
      address = divbox.childNodes[a].id;
      break;
    }
  }
  for(var a = divbox.childNodes.length-1; a>=0; a--) {
    // find first instruction and get the address
    if (divbox.childNodes[a].className == "instruction") {
      endaddress = divbox.childNodes[a].id;
      break;
    }
  }
  return new Node(divbox, address, endaddress);
}

// Edge class
function Edge(nn1, nn2, color, reversed) {
  // only place that uses graph locally
  this.start = graph.getNode(nn1);
  this.end = graph.getNode(nn2);
  this.color = color;
  this.reversed = reversed;
  if(this.start == null || this.end == null) {
    geohot.debug("incomplete edge: "+nn1+", "+nn2);
  }
}

Edge.prototype.drawLine = function(ox,ox2, oy) {

  var line=new Array();

  var sy=graph.levelboxes[this.end.level].offsetTop+15;
  var rey=graph.levelboxes[this.start.level].offsetTop+15 + this.start.yheight;


  var da = (this.end.box != null);

  if(!this.reversed) {

    line.push({x: this.end.xcenter+ox, y: sy});
    // Fixed based on the margins
    line.push({x: this.end.xcenter+ox, y: oy+(sy-15)}); //come up
    line.push({x: this.start.xcenter+ox2, y: oy+(sy-15)}); //come over
    line.push({x: this.start.xcenter+ox2, y: rey});  //and up
    //alert(sx + ", " +sy + " - " + ex + ", " + ey);
    drawlinearray(line, this.color, da);
  } else {


    line.push({x: this.end.xcenter+ox, y: sy});
    // Fixed based on the margins
    line.push({x: this.end.xcenter+ox, y: oy+(sy-15)}); //come up

    line.push({x: this.end.xcenter+(this.end.xwidth/2)+15, y: oy+(sy-15)}); //come over to the side
    line.push({x: this.end.xcenter+(this.end.xwidth/2)+15, y: rey+15});  //come down

    line.push({x: this.start.xcenter+ox2, y: rey+15}); //come over
    line.push({x: this.start.xcenter+ox2, y: rey});  //and up
    //alert(sx + ", " +sy + " - " + ex + ", " + ey);
    drawlinearray(line, this.color, da);
  }
}

// reverse the Edge for step 1 sugiyama
Edge.prototype.reverseEdge = function() {
  //if(this.reversed) return;

  var a = this.start;
  this.start = this.end;
  this.end = a;
  //this.reversed = !this.reversed; // allowed?

  this.reversed = true;   //always keep it marked

  //geohot.debug("reversed edge between "+this.start.address+" and "+this.end.address);
};

Edge.prototype.connects = function(n1, n2) {
  if(n1==this.start && n2==this.end) return true;
  //if(n1==this.end && n2==this.start) return true;
  return false;
};


// Graph class
//graph.nodes = {};   // graph nodes is an object mapping nodenames to Nodes
//graph.edges = [];   // edges is an array of all the edges.
// trade crappy algos and KR for easy to understand code. it's the google way :-)


//**************************Graph Level Functions****************************

function Graph() {
  this.nodes = {};
  this.edges = [];

  this.levels = {};
  this.levelboxes = [];
}

// get the max level of a node in the graph
Graph.prototype.maxLevel = function() {
  var max=0;
  for(a in this.nodes) {
    if(this.nodes[a].level > max)
      max = this.nodes[a].level;
  }
  return max;
};

Graph.prototype.inLevel = function(levelNumber) {
  var levelarray = [];
  for(a in this.nodes) {
    if (this.nodes[a].level == levelNumber) {
      levelarray.push(this.nodes[a]);
    }
  }
  return levelarray;
}

// draw a level and add it to main
// does the reordering here
Graph.prototype.drawLevel = function(levelNumber) {

  this.levels[levelNumber] = [];

  // Get an array of things in this level
  var levelarray = this.inLevel(levelNumber);
  var xcenterarray = [];

  if(levelarray.length == 0) {
    geohot.debug("blank level at "+levelNumber);
    return;
  }

// Generate ideal xcenters
  for(var i=0; i<levelarray.length; i++) {
    var ch=this.getChildren(levelarray[i]);
    if(ch.length == 0) {
      levelarray[i].xcenter = 0;
    } else {
      var avgplace = 0;
      for(n in ch) {
        //geohot.debug("xcenter of "+ch[n].address+" is "+ch[n].xcenter);
        avgplace += ch[n].xcenter;
      }
      levelarray[i].xcenter = avgplace/(ch.length);
    }
  }

// Offset for real world and add to xcenterarray
  for(var i=0; i<levelarray.length; i++) {
   /* if(levelarray[i].xcenter < xdistance) {
      levelarray[i].xcenter = xdistance;
    }
    xdistance += levelarray[i].xwidth;*/
    xcenterarray.push(levelarray[i].xcenter);
  }

  //geohot.debug("L"+levelNumber+" ",xcenterarray);
  xcenterarray.sort();

  var tlevel = document.createElement("div");
  tlevel.id = "L" + levelNumber;
  tlevel.className = "level";

  var xdistance = 0;  // distance used up, place in order

  for(var i=0; i<xcenterarray.length; i++) {
    for(var j=0; j<levelarray.length; j++) {
      if(xcenterarray[i] == levelarray[j].xcenter) {
        if(levelarray[j].xcenter < xdistance) {
          //this.getNode(levelarray[j].address).xcenter = xdistance;
          levelarray[j].xcenter = xdistance;
        } else {
          xdistance = levelarray[j].xcenter;
        }
        //geohot.debug( levelarray[j].address+": "+levelarray[j].xcenter+" "+xdistance);
        xdistance += levelarray[j].xwidth;
        if(levelarray[j].box != null)
          tlevel.appendChild( levelarray[j].box );
        this.levels[levelNumber].push( levelarray[j] );
        levelarray.splice(j,1);   // Remove the node since it's placed
        break;
      }
    }
  }
  this.levelboxes[levelNumber] = tlevel;
  return tlevel;
  //document.getElementById("mega").appendChild(tlevel);
};

// Create dummy nodes
Graph.prototype.completeLevels = function() {
  var edgesToAdd = [];
  for(e in this.edges) {
    var distance = this.edges[e].start.level-this.edges[e].end.level;
    if(distance<0) geohot.debug("WTF backward edge");
    if(distance > 1) {
      var startnode = this.edges[e].start;
      var endnode = this.edges[e].end;
      var mycolor = this.edges[e].color;
      var myreversed = this.edges[e].reversed;

      this.deleteEdge( this.edges[e] );

      var newlevel = startnode.level-1;
      var lastlevelname = startnode.address;
      while(newlevel != endnode.level) {
        /*var dumbbox = document.createElement("div");
        dumbbox.className = "codebox";
        //dumbbox.innerHTML = "P"+lastlevelname+" "+endnode.address;
        dumbbox.innerHTML = " <br/> ";*/

        var nn=this.addNode(new Node(null, "P"+lastlevelname, "P"+lastlevelname));
        nn.level=newlevel;
        nn.xwidth=20;   //make believe nodes are smaller
        this.addEdge(new Edge(lastlevelname, "P"+lastlevelname, mycolor, myreversed));

        lastlevelname = "P"+lastlevelname;
        newlevel--;
      }
      this.addEdge(new Edge(lastlevelname, endnode.address, mycolor, myreversed));

    }
  }
};

//**************************Graph Relative Functions****************************

// pass in a Node object
// returns an array of parent nodes
Graph.prototype.getParents = function(node) {
  var retarray = [];
  for(var a=0; a<this.edges.length; a++) {
    if (this.edges[a].end == node)
      retarray.push(this.edges[a].start);
  }
  return retarray;
};

// pass in a Node object
// returns an array of child nodes
Graph.prototype.getChildren = function(node) {
  var retarray = [];
  for(var a=0; a<this.edges.length; a++) {
    if (this.edges[a].start == node)
      retarray.push(this.edges[a].end);
  }
  return retarray;
};

//****************************Graph Edge Functions******************************

// get edge that connects n1 and n2
// returns Edge object
Graph.prototype.getEdge = function(n1, n2) {
  for(var a=0; a<this.edges.length; a++) {
    if(this.edges[a].connects(n1, n2))
      return this.edges[a];
  }
  //geohot.debug("no such edge connecting "+n1.address+" "+n2.address);
  return null;
};

Graph.prototype.deleteEdge = function(edge) {
  //geohot.debug("edge deleted: "+edge.start.address+" "+edge.end.address);
  this.edges.splice(this.edges.indexOf(edge), 1);
}

// pass in an Edge object
Graph.prototype.addEdge = function(edge) {
  this.edges.push(edge);
  return this.edges[this.edges.length-1];
};

// pass in an array of lines
Graph.prototype.addEdgeList = function(edgelist) {
  //geohot.debug(typeof(edgelist));
  for(var a=0; a<edgelist.length; a++) {
    if(this.getEdge(    //no dupes
        this.getNode(edgelist[a].getAttribute("from")),
        this.getNode(edgelist[a].getAttribute("to")))==null) {
      this.addEdge(new Edge(edgelist[a].getAttribute("from"),
                            edgelist[a].getAttribute("to"),
                            edgelist[a].getAttribute("color"),
                            false));
    }
  }
};

//****************************Graph Node Functions******************************

// pass in a Node object
Graph.prototype.addNode = function(node) {
  this.nodes[node.address] = node;
  return this.nodes[node.address];
};

Graph.prototype.addNodeList = function(nodelist) {
  for(var a=0; a<nodelist.length; a++) {
    if(a == 0)
      this.topNode = this.addNode( NodeFromDiv(nodelist[a]) );
    else
      this.addNode( NodeFromDiv(nodelist[a]) );
    //geohot.debug("added node: ", a);
  }
};

// takes a nodename and returns that node if it exists
// for pretty abstraction
Graph.prototype.getNode = function(nodename) {
  if(this.nodes[nodename] != null) return graph.nodes[nodename];
  // check the end addresses too
  for(a in this.nodes) {
    if(this.nodes[a].endaddress == nodename) return this.nodes[a];
  }
  return null;
};


//******************************************************************************
//************************END GRAPH CLASS, THIS IS CODE*************************
//******************************************************************************

// do the actual drawing on the screen
function levelPlace() {
  document.getElementById("mega").innerHTML =
    "<canvas id=\"line_canvas\" width=\"3000px\" height=\"3000px\"></canvas>";
  /*for(var a=graph.maxLevel(); a>=0; a--) {
    graph.drawLevel(a);
  }*/
  var out = [];
  for(var a=0; a<=graph.maxLevel(); a++) {
    out.push(graph.drawLevel(a));
  }

  var mega = document.getElementById("mega");
  for(var a=(out.length-1); a>=0; a--) {
    mega.appendChild(out[a]);
  }
}

function childlessNodes() {
  var cn = [];
  for(w in graph.nodes) {
    if(graph.getChildren(graph.nodes[w]).length == 0)
      cn.push(graph.nodes[w]);
  }
  return cn;
}

// place all childless nodes then recurse upwards
var ltl = 20;

function yPlace(nodeArray, levelNumber) {
  if(levelNumber == ltl) return; //no crash test

  if(nodeArray.length == 0) return;
  //geohot.debug("placing level "+levelNumber+" with "+stringNodeArray(nodeArray));
  var nextLevel=[];
  for(a in nodeArray) {
    nodeArray[a].level = levelNumber;
    nextLevel = nextLevel.concat(graph.getParents(nodeArray[a]));
  }
  arrayMakeUnique(nextLevel);
  yPlace(nextLevel, levelNumber+1);
}

//apply xcenters
function xPlace() {
  for(var ln=0; ln<=graph.maxLevel(); ln++) {
    var la = graph.levels[ln];
    var xcarray = [];
    var last=0;
    for(var b=0; b<la.length; b++) {
      xcarray.push(la[b].xcenter);
      var calcpad;
      //la[b].xcenter = la[b].xcenter + la[b].xwidth;   // Update to center
      la[b].xcenter += 500;   //shift over
      if(la[b].box != null) {
        la[b].xwidth = la[b].box.offsetWidth; // Update to real width
        la[b].yheight = la[b].box.offsetHeight;

        calcpad = (la[b].xcenter-(la[b].xwidth)/2)-last;
        last = la[b].xcenter+(la[b].xwidth/2);    //right edge

        la[b].box.style.marginLeft=calcpad+"px";
      }

    }
    //geohot.debug("L"+ln+": ", xcarray);
  }
}

// set a lineage property on the nodes, doesn't really matter long term
// start at the top node and do this recursively

/*
switching 401024 and 401064
switching 401054 and 401054
switching 401024 and 40102c
switching 4010a0 and 4010cc
switching 4010c0 and 4010c0
switching 400580 and 400580
switching 4005e4 and 4005ec
*/

function removeLoops(n) {
  //geohot.debug("called on node "+n.address+" with ttl "+ttl);
  var children = graph.getChildren(n);
  for(a in children) {
    //geohot.debug("placing child "+children[a].address);
    //geohot.debug(stringNodeArray(children[a].lineage), stringNodeArray(n.lineage));

    if( n.lineage.indexOf(children[a])!=-1 ) {
      // this is a strange loop, reverse the edge
      // the reverse edge parent already exists in the lineage, so we don't have to worry about that
      if(children[a] == n) {  // This is a local loop
        graph.deleteEdge(graph.getEdge(n, children[a]));

        //graph.addEdge

      }
      else
        graph.getEdge(n, children[a]).reverseEdge();
    }
    else {
      children[a].lineage = children[a].lineage.concat([n], n.lineage);
      removeLoops(children[a]);
    }
  }
}

function nodeCmp(a,b) {
  return a.address>b.address;
}


function linePlace() {
  /*for(e in graph.edges) {
    graph.edges[e].drawLine();
  }*/
  for(ln in graph.levels) {
    for(var nn=0; nn<graph.levels[ln].length; nn++) {
      var tn = graph.levels[ln][nn];
      var tnp = graph.getParents(tn);
      var ox = -1*(((tnp.length-1)*16)/2);

      tnp.sort(nodeCmp);

      var yoff = 0;
      /*if(ox<-15) {
        graph.levelboxes[ln].style.marginTop=(-1*ox)+"px";
        yoff=ox;
      }*/

      //geohot.debug(stringNodeArray(tnp));

      for(n in tnp) {
        /*if(myparents.indexOf(tnp[n])==-1)
          continue;*/
        //geohot.debug("drawing edge: " +tnp[n].address);
        var tnpc = graph.getChildren(tnp[n]);
        if(tnpc.length == 1)
          graph.getEdge(tnp[n], tn).drawLine(ox,0,yoff+ox/2);
        else {
          if( ( (tnpc[0] != tn) ^ (graph.levels[ln].indexOf(tnpc[0]) < graph.levels[ln].indexOf(tnpc[1]) ) ) || graph.levels[ln].indexOf(tnpc[1])==-1 )
            graph.getEdge(tnp[n], tn).drawLine(ox,-8,yoff+ox/2);
          else
            graph.getEdge(tnp[n], tn).drawLine(ox,8,yoff+ox/2);
        }
        ox+=16;
        //alert(" ");
      }
    }
    //alert(" ");
  }
}

//****************************Graph Main Function*******************************

// this should be easy now, lol :P

function graphDraw(edgelist, divlist) {
  //geohot.debug(divlist, edgelist);
  graph = new Graph;
  graph.addNodeList(divlist);
  graph.addEdgeList(edgelist);
  //geohot.debug(graph.nodes, graph.edges);

  removeLoops(graph.topNode);

  yPlace(childlessNodes(),0);

  graph.completeLevels();

  levelPlace();

  xPlace();

  for(e in graph.edges) {
    if(graph.edges[e].reversed) graph.edges[e].reverseEdge();
  }

  linePlace();
}
