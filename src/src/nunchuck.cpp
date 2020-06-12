#include <Arduino.h>
#include "nunchuck.h"

/*
   Duplo Lego Car
   Based on Tod E. Kurt, http://thingm.com/
*/

  uint8_t nunchuck_buf[6];  

Nunchuck::Nunchuck() 
{
  //_MQTTClient = MQTTClient;

  //nunchuck_init(); // send the initilization handshake

  // Log("Wii Nunchuck ready\n");
}

MotorXY Nunchuck::Loop()
{
  nunchuck_get_data();

  accx = nunchuck_accelx(); // ranges from approx 70 - 182
  accy = nunchuck_accely(); // ranges from approx 65 - 173
  zbut = nunchuck_zbutton();
  cbut = nunchuck_cbutton();
  joyx = nunchuck_joyx();
  joyy = nunchuck_joyy();

  //Log("accx: "); Log((byte)accx, DEC);
  //Log("\taccy: "); Log((byte)accy, DEC);
  //Log("\tzbut: "); Log((byte)zbut, DEC);
  //Log("\tcbut: "); Log((byte)cbut, DEC);
  //Log("\tjoyx: "); Log((byte)joyx, DEC);
  //Log("\tjoyy: "); Log((byte)joyy, DEC);

  // motor_x = joyx;
  // motor_y = joyy;

  //setMotorsNunChuck(joyx, joyy);

  int motor_x = map(joyx, 0, 255, -1, 1);
  int motor_y = map(joyy, 0, 255, -1, 1);

  MotorXY motorXY;
  motorXY.motor_x = motor_x;
  motorXY.motor_y = motor_y;
  motorXY.fromMQTT = false;

  // Log("joyx: " +  joyx);
  // Log("joyy: " + joyy);

  // Log("mapx: " +  motor_x);
  // Log("mapy: "+ motor_y);

  return motorXY;
}


/*
 * Nunchuck functions  -- Talk to a Wii Nunchuck
 *
 * This library is from the Bionic Arduino course : 
 *                          http://todbot.com/blog/bionicarduino/
 *
 * 2007-11 Tod E. Kurt, http://todbot.com/blog/
 *
 * The Wii Nunchuck reading code originally from Windmeadow Labs
 *   http://www.windmeadow.com/node/42
 */




// initialize the I2C system, join the I2C bus,
// and tell the nunchuck we're talking to it
void Nunchuck::nunchuck_init()
{
  Log("Nunchuck initialise");

    //Wire.begin();                 // join i2c bus as master
    Wire.beginTransmission(0x52); // transmit to device 0x52
#if (ARDUINO >= 100)
    Wire.write((uint8_t)0x40); // sends memory address
    Wire.write((uint8_t)0x00); // sends sent a zero.
#else
    Wire.send((uint8_t)0x40); // sends memory address
    Wire.send((uint8_t)0x00); // sends sent a zero.
#endif
    Wire.endTransmission(); // stop transmitting
}

// Send a request for data to the nunchuck
// was "send_zero()"
void Nunchuck::nunchuck_send_request()
{
    Wire.beginTransmission(0x52); // transmit to device 0x52
#if (ARDUINO >= 100)
    Wire.write((uint8_t)0x00); // sends one byte
#else
    Wire.send((uint8_t)0x00); // sends one byte
#endif
    Wire.endTransmission(); // stop transmitting
}

// Encode data to format that most wiimote drivers except
// only needed if you use one of the regular wiimote drivers
char Nunchuck::nunchuk_decode_byte(char x)
{
    x = (x ^ 0x17) + 0x17;
    return x;
}

// Receive data back from the nunchuck,
// returns 1 on successful read. returns 0 on failure
int Nunchuck::nunchuck_get_data()
{
    int cnt = 0;
    Wire.requestFrom(0x52, 6); // request data from nunchuck
    while (Wire.available())
    {
        // receive byte as an integer
#if (ARDUINO >= 100)
        nunchuck_buf[cnt] = nunchuk_decode_byte(Wire.read());
#else
        nunchuck_buf[cnt] = nunchuk_decode_byte(Wire.receive());
#endif
        cnt++;
    }
    nunchuck_send_request(); // send request for next data payload
    // If we recieved the 6 bytes, then go print them
    if (cnt >= 5)
    {
        return 1; // success
    }
    return 0; //failure
}

// Print the input data we have recieved
// accel data is 10 bits long
// so we read 8 bits, then we have to add
// on the last 2 bits.  That is why I
// multiply them by 2 * 2
void Nunchuck::nunchuck_print_data()
{
    int i = 0;
    int joy_x_axis = nunchuck_buf[0];
    int joy_y_axis = nunchuck_buf[1];
    int accel_x_axis = nunchuck_buf[2]; // * 2 * 2;
    int accel_y_axis = nunchuck_buf[3]; // * 2 * 2;
    int accel_z_axis = nunchuck_buf[4]; // * 2 * 2;

    int z_button = 0;
    int c_button = 0;

    // byte nunchuck_buf[5] contains bits for z and c buttons
    // it also contains the least significant bits for the accelerometer data
    // so we have to check each bit of byte outbuf[5]
    if ((nunchuck_buf[5] >> 0) & 1)
        z_button = 1;
    if ((nunchuck_buf[5] >> 1) & 1)
        c_button = 1;

    if ((nunchuck_buf[5] >> 2) & 1)
        accel_x_axis += 1;
    if ((nunchuck_buf[5] >> 3) & 1)
        accel_x_axis += 2;

    if ((nunchuck_buf[5] >> 4) & 1)
        accel_y_axis += 1;
    if ((nunchuck_buf[5] >> 5) & 1)
        accel_y_axis += 2;

    if ((nunchuck_buf[5] >> 6) & 1)
        accel_z_axis += 1;
    if ((nunchuck_buf[5] >> 7) & 1)
        accel_z_axis += 2;

    // Log(i,DEC);
    // Log("\t");

    // Log("joy:");
    // Log(joy_x_axis,DEC);
    // Log(",");
    // Log(joy_y_axis, DEC);
    // Log("  \t");

    // Log("acc:");
    // Log(accel_x_axis, DEC);
    // Log(",");
    // Log(accel_y_axis, DEC);
    // Log(",");
    // Log(accel_z_axis, DEC);
    // Log("\t");

    // Log("but:");
    // Log(z_button, DEC);
    // Log(",");
    // Log(c_button, DEC);

    // Log("\r\n");  // newline
    i++;
}

// returns zbutton state: 1=pressed, 0=notpressed
int Nunchuck::nunchuck_zbutton()
{
    return ((nunchuck_buf[5] >> 0) & 1) ? 0 : 1; // voodoo
}

// returns zbutton state: 1=pressed, 0=notpressed
int Nunchuck::nunchuck_cbutton()
{
    return ((nunchuck_buf[5] >> 1) & 1) ? 0 : 1; // voodoo
}

// returns value of x-axis joystick
int Nunchuck::nunchuck_joyx()
{
    return nunchuck_buf[0];
}

// returns value of y-axis joystick
int Nunchuck::nunchuck_joyy()
{
    return nunchuck_buf[1];
}

// returns value of x-axis accelerometer
int Nunchuck::nunchuck_accelx()
{
    return nunchuck_buf[2]; // FIXME: this leaves out 2-bits of the data
}

// returns value of y-axis accelerometer
int Nunchuck::nunchuck_accely()
{
    return nunchuck_buf[3]; // FIXME: this leaves out 2-bits of the data
}

// returns value of z-axis accelerometer
int Nunchuck::nunchuck_accelz()
{
    return nunchuck_buf[4]; // FIXME: this leaves out 2-bits of the data
}
