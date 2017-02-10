using namespace::boost::asio;  // save tons of typing
#include <iostream>
using std::cin;

// These are the values our port needs to connect
#ifdef _WIN32
// windows uses com ports, this depends on what com port your cable is plugged in to.
    const char *PORT = "COM3";
#else
// *nix com ports
    const char *PORT = "dev/ttyS3";
#endif
// Note: all the following except BAUD are the exact same as the default values

// what baud rate do we communicate at
serial_port_base::baud_rate BAUD(19200);
// how big is each "packet" of data (default is 8 bits)
serial_port_base::character_size C_SIZE( 8 );
// what flow control is used (default is none)
serial_port_base::flow_control FLOW( serial_port_base::flow_control::none );
// what parity is used (default is none)
serial_port_base::parity PARITY( serial_port_base::parity::none );
// how many stop bits are used (default is one)
serial_port_base::stop_bits STOP( serial_port_base::stop_bits::one );

int main()
{
    // create the I/O service that talks to the serial device
    io_service io;
    // create the serial device, note it takes the io service and the port name
    serial_port port( io, PORT );

    // go through and set all the options as we need them
    // all of them are listed, but the default values work for most cases
    port.set_option( BAUD );
    port.set_option( C_SIZE );
    port.set_option( FLOW );
    port.set_option( PARITY );
    port.set_option( STOP );

    // buffer to store commands
    // this device reads 8 bits, meaning an unsigned char, as instructions
    // varies with the device, check the manual first
    unsigned char command[1] = {0};

    // read in user value to be sent to device
    int input;
    cin >> input;

    // Simple loop, since the only good values are [0,255]
    //  break when a negative number is entered.
    // The cast will convert too big numbers into range.
    while( input >= 0 )
    {
        // convert our read in number into the target data type
        command[0] = static_cast<unsigned char>( input );

        // this is the command that sends the actual bits over the wire
        // note it takes a stream and a asio::buffer
        // the stream is our serial_port
        // the buffer is constructed using our command buffer and
        //  the number of instructions to send
        write( port, buffer( command, 1 ) );

        // read in the next input value
        cin >> input;
    }

    // all done sending commands
    return 0;
}
