<?php
/*
 * Created on May 7, 2013
 *
 * Erstellt:			Lehner Matthias
 * Beschreibung: TCP Socketverbindung, als Client ausgeführt
*/
	//Positions of the types
	define(CMD_POS, 0);
	define(TYPE_POS, 1);
	//define(MSG_POS, 2);
	//Command -----------------------
	define(CMD, 0x01);
	//TYPE_CMD
	define(WALK, 0x01);
	//Walk Commands
	define(FORW_POS, 2);
	define(BACKW_POS, 3);
	define(LEFT_POS, 4);
	define(RIGHT_POS, 5);
	//---------------------
	//Rotate Commands
	define(ROT, 0x02);
	define(LEFT, 0x01);
	define(LEFT_POS, 2);
	define(RIGHT, 0x02);
	define(RIGHT_POS, 3);
	//----------------------
	define(HEIGTH, 0x03);
	define(BAT_STAT, 0x64);
	//Text
	define(TXT, 0x02);
	//TYPE_TXT
	define(NORMAL, 0x01);
	define(WARNING, 0x02);
	define(ERROR, 0x03);
	//Receive
	define(ACK, 0x06);
	define(NAK, 0x15);
	
	$host="127.0.0.1";
   $port = 50000;

	//create struct for protokoll

   $cmd = array("cmd" => "\x01");
   $cmd[cmd] = array("walk" => "\x01");
   $tx_package;
   $rx_package;
	
	$protokoll[COM_POS] = CMD;
	$protokoll[TYPE_POS] = WALK;

   if(!isset($_POST["Message"])) 
         die("no command specified!");

   echo "<b>Send Command:</b><br/>";

	switch ($_POST["Message"]) {
		case "move-forward":
			echo "move forward<br />";
         $tx_package  = "\x01";
         $tx_package .= "\x01";
         $tx_package .= "move-forward";
			break;
			
		case "move-reverse":
         $tx_package  = "\x01";
         $tx_package .= "\x01";
         $tx_package .= "move-reverse";
			echo "move reverse<br />";
			break;
			
		case "move-left":
         $tx_package  = "\x01";
         $tx_package .= "\x01";
         $tx_package .= "move-left";
			echo "move left  <br />";
			break;
			
		case "move-right":
         $tx_package  = "\x01";
         $tx_package .= "\x01";
         $tx_package .= "move-right";
			echo "move right <br />";
			break;

		case "turn-left":
         $tx_package  = "\x01";
         $tx_package .= "\x01";
         $tx_package .= "turn-left";
			echo "turn left  <br />";
			break;
			
		case "turn-right":
         $tx_package  = "\x01";
         $tx_package .= "\x01";
         $tx_package .= "turn-right";
			echo "turn right <br />";
			break;
			
		case "wakeup":
         $tx_package  = "\x01";
         $tx_package .= "\x01";
         $tx_package .= "wakeup";
			echo "wakeup <br />";
			break;
			
		case "sleep":
         $tx_package  = "\x01";
         $tx_package .= "\x01";
         $tx_package .= "sleep";
			echo "sleep <br />";
			break;			

		case "start":
	exec('cd /home/robsoft3g/trunk/Hexapod/Debug/ && ./Hexapod > /dev/null&');
	echo "server started...";
			break;
			
		case "shutdown":
         $tx_package  = "\x01";
         $tx_package .= "\x01";
         $tx_package .= "shutdown";
			echo "shutdown <br />";
			break;
			
                case "webcam-start":
        exec('/usr/local/bin/mjpg_streamer -i "/usr/local/lib/input_uvc.so -n -r 640x360 -f 5 -q 60" -o "/usr/local/lib/output_http.so -n -w /usr/local/www -p 8080" > /dev/null&');
        echo "starting webcam...";
                        break;

                case "webcam-stop":
        exec('kill -9 $(pidof mjpg_streamer) >/dev/null 2>&1');
        echo "stoping webcam...";
                        break;

		case "tripod-gait":
         $tx_package  = "\x01";
         $tx_package .= "\x01";
         $tx_package .= "tripod-gait";
			echo "tripod-gait <br />";
			break;
		
		case "wave-gait":
         $tx_package  = "\x01";
         $tx_package .= "\x01";
         $tx_package .= "wave-gait";
			echo "wave-gait <br />";
			break;
			
		case "ripple-gait":
         $tx_package  = "\x01";
         $tx_package .= "\x01";
         $tx_package .= "ripple-gait";
			echo "ripple-gait <br />";
			break;
			
		default:
			echo "unkown message! <br />";
			break;
		}

	if (strlen($tx_package) != 0) {
     // create socket
     $socket = socket_create(AF_INET, SOCK_STREAM, 0) or
            die("Could not create socket\n");   	

     // connect to server
     socket_connect($socket, $host, $port) or 
            die("Could not connect to server\n");

     // send package to server 
	  socket_write($socket, $tx_package) or 
            die("Could not send data to server\n");

	  // wait for answer	
	  $rx_package = socket_read ($socket, 1024) or 
            die("Could not read server response\n");
     echo "<b>Answer form Server:</b><br/>$rx_package";
	
    // close socket
    socket_close($socket);
	}
?>
