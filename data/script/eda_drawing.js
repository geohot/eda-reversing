// eda_drawing.js -- Apr 23, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

//*****************************************************************************
//***************************LOW LEVEL DRAWING*********************************
//*****************************************************************************

function drawarrow(x,y,c) {
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

function drawline(x,y,w,h,c) {
  animatedctx=document.getElementById('line_canvas').getContext('2d');
  animatedctx.beginPath();
  //animatedctx.fillStyle=rgba(255,255,255,0);
  if(c=="red")
    animatedctx.fillStyle='rgba(255,0,0,128)';
  else if(c=="green")
    animatedctx.fillStyle='rgba(0,128,0,128)';
  else if(c=="blue")
    animatedctx.fillStyle='rgba(0,0,255,128)';
  if(w<0) {
    x=x+w;    //move x left
    w=-w;
  }
  else
    w++;
  if(h<0) {
    y=y+h;    //move y up
    h=-h;
  }
  else
    h++;

  animatedctx.rect(x,y,w,h);
  animatedctx.fill();
}

function drawlinearray(a,c,da) {    //a is {x,y}, c is the color
  //this should draw an arrow too
  if(da)
    drawarrow(a[0].x,a[0].y,c);

  for(var i=1;i<a.length;i++) {
    if(a[i].x==a[i-1].x)    //x is the same
      drawline(a[i-1].x, a[i-1].y, 0, a[i].y-a[i-1].y,c);

    if(a[i].y==a[i-1].y)    //y is the same
      drawline(a[i-1].x, a[i-1].y, a[i].x-a[i-1].x, 0,c);
  }
}