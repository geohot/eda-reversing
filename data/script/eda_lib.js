// eda_lib.js -- May 3, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

// Library functions for EDAs javascript stuff
// should eventually replace eda_server.js

// xx is EDAs quick communication lib
// response options are html, xml, and status
// it is blocking, since the server is blocking
// fully REST compliant :-)
function xx(verb, loc, dat) {
  var req = new XMLHttpRequest();
  req.open(verb, loc, false);
  req.send(dat);
  while(req.readyState!=4) { }
  var ret = {
    html: req.responseText,
    xml: req.responseXML,
    status: req.status};
  return ret;
}