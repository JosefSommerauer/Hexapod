<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta name=viewport content="width=device-width, initial-scale=1, maximum-scale=1">
<meta name=apple-mobile-web-app-capable content=yes>
<meta name=apple-mobile-web-app-status-bar-style content=black>



<title>HexaMote</title>

<link rel="stylesheet" href="css/style.css" type="text/css" />
<link rel="icon" href="favicon.ico" type="image/x-icon">

</head>

<script src="/scripts/jquery-1.9.1.min.js"></script>

<script type="text/javascript">
    /* THIS FUNCTION IS TO FETCH INT PARAMETER VALUES */
    function getIntParametera(param) {
        var val = document.URL;
        var url = val.substr(val.indexOf(param))  
        var n=parseInt(url.replace(param+"=",""));
        return n; 
    }

    /*THIS FUNCTION IS TO FETCH STRING PARAMETER*/
    function getStringParameter(param) {
        var val = document.URL;
        var url = val.substr(val.indexOf(param))  
        var n=url.replace(param+"=","");
        return n; 
    }

/*********** CODE FOR WEBCAM **********/

/* Copyright (C) 2007 Richard Atterer, richardÂ©atterer.net
   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License, version 2. See the file
   COPYING for details. */

var imageNr = 0; // Serial number of current image
var finished = new Array(); // References to img objects which have finished downloading
var paused = false;

function createImageLayer() {
	try {
		var img = new Image();
		img.style.position = "absolute";
		img.style.zIndex = -1;
		img.onload = imageOnload;
		img.src = "http://<?php echo $_SERVER['SERVER_ADDR'] ?>:8080/?action=snapshot&n=" + (++imageNr);
		var webcam = document.getElementById("webcam");
		webcam.insertBefore(img, webcam.firstChild);
	}
	catch(err) {
		console.log(err);
	}
}

// Two layers are always present (except at the very beginning), to avoid flicker
function imageOnload() {
  this.style.zIndex = 1; //imageNr; // Image finished, bring to front!
  while (finished.length > 1) {
    var del = finished.shift(); // Delete old image(s) from document
    del.parentNode.removeChild(del);
  }
  finished.push(this);
  if (!paused) createImageLayer();
}

function imageOnclick() { // Clicking on the image will pause the stream
  paused = !paused;
  if (!paused) createImageLayer();
}


function imageStart() { // Clicking on the image will pause the stream
  paused = false;
  createImageLayer();
}

function imageStop() { // Clicking on the image will pause the stream
  paused = true;
  while (finished.length > 0) {
    var del = finished.shift(); // Delete old image(s) from document
    del.parentNode.removeChild(del);
  }
}

</script>

<script type='text/javascript' charset='utf-8'>
    function hideAddressBar(){
  if(document.documentElement.scrollHeight<window.outerHeight/window.devicePixelRatio)
    document.documentElement.style.height=(window.outerHeight/window.devicePixelRatio)+'px';
  setTimeout(window.scrollTo(1,1),0);
}

//if(window.innerHeight <= 300) {
//   window.addEventListener("load",function(){hideAddressBar();});
//   window.addEventListener("orientationchange",function(){hideAddressBar();});
//}

</script> 

<script>
/********** CODE FOR CONTROLBUTTONS **********/

var timer;

//$(this).attr('value')

function SendMessage(cmd) {
    var posting = $.post("Socket.php", {Message: cmd}) 
    .fail(function() { 
      $( "#result" ).empty().append("php server timeout");
    })
    .done(function( data ) {
      $( "#result" ).empty().append(data);
      imageStart();
    });
}

$(document).keydown(function (event) {
	switch (event.keyCode) {
	   case 65: /*a*/
	     SendMessage("move-left");
	     break;
           case 87: /*w*/
             SendMessage("move-forward");
             break;
           case 68: /*d*/ 
             SendMessage("move-right");
             break;
	   case 83: /*s*/
             SendMessage("move-reverse");
             break;
           case 81: /*q*/
             SendMessage("turn-left");
             break;
           case 69: /*e*/
             SendMessage("turn-right");
             break;

	   default:
	     break;
	}	
});

$(document).ready(function(){
  $("button").mousedown(function(){
     var msg = $( this ).attr('value');
     SendMessage(msg);
     timer = setInterval(function(){SendMessage(msg)},500);

     if(msg == "webcam-start") {window.setTimeout(
     	function(){
     		imageStart()
     		console.log("start")
     	},600);};
     if(msg == "webcam-stop")  {imageStop()};

  }).mouseup(function(){
     clearInterval(timer);
  }).mouseout(function(){
     clearInterval(timer);
  });
});
</script>


<body style="border:0;margin:0;padding:0" onload="createImageLayer();">


<div id="page">

<div id="webcam"><img src="http://<?php echo $_SERVER['SERVER_ADDR'] ?>/webcam-dummy.jpg"></div>


<div id="control">
 <div>
    <div><button id="turn-left"  name="Message" value="turn-left"     title="turn-left" > &#x21BA; </button></div>
    <div><button id="turn-right" name="Message" value="turn-right"    title="turn-right"> &#x21BB; </button></div>
    <div><button id="forward"    name="Message" value="move-forward"  title="forward"   > &#x21E1; </button></div>
    <div><button id="left"       name="Message" value="move-left"	    title="left"      > &#x21E0; </button></div>
    <div><button id="right"      name="Message" value="move-right"    title="right"     > &#x21E2; </button></div>
    <div><button id="reverse"    name="Message" value="move-reverse"  title="reverse"   > &#x21E3; </button></div>
 </div>
 
 <div id="server-cmd">
    <div><button id="start"        name="Message" value="start"        title="start"   >start server</button></div>
    <div><button id="shutdown"     name="Message" value="shutdown"     title="shutdown">shutdown server</button></div>
    <div><button id="wakeup"       name="Message" value="wakeup"       title="wakeup"  >wakeup</button></div>
    <div><button id="sleep"        name="Message" value="sleep"        title="sleep"   >sleep</button></div>
    <div><button id="webcam-start" name="Message" value="webcam-start" title="webcam-start">start webcam</button></div>
    <div><button id="webcam-stop"  name="Message" value="webcam-stop"  title="webcam-stop" >stop webcam</button></div>
 </div>
 
<!--
 <div>
    <div><button id="tripod gait" name="Message" value="tripod-gait" title="tripod gait">tripod gait</button></div>
    <div><button id="wave gait"    name="Message" value="wave-gait"    title="wave gait"   >wave gait</button></div>
    <div><button id="ripple gait" name="Message"  value="ripple-gait"  title="ripple gait" >ripple gait</button></div>
 </div>
-->
 
 <div id="result">
    controll keys:
    <ul>
	<li><b>[q]</b> => turn left</li>
	<li><b>[e]</b> => turn right</li>
	<li><b>[w]</b> => move forward</li>
	<li><b>[a]</b> => move left</li>
	<li><b>[s]</b> => move reverse</li>
	<li><b>[d]</b> => move right</li>
    </ul>
 </div>

</div>
</div>


</body>
</html>

