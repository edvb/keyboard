#include "TrackPoint.h"
#include "config.h"

#define LEN(X) (sizeof(X) / sizeof((X)[0]))

bool toggleBind = false;
int currLayer = 0;
int prevLayer = 0;
int desiredLayer = 1;

byte row[ROWS] = ROW_PINS;
byte col[COLS] = COL_PINS;

int mouse = 0;
int ispress[] = {0,0,0};
int key[] = {0,0,0,0,0,0};
int mod[] = {0,0,0,0};

const char mouseStates[] = {
	'L', MOUSE_LEFT,
	'M', MOUSE_MIDDLE,
	'R', MOUSE_RIGHT
};

TrackPoint trackpoint(CLOCK, DATA, RESET, false);

void setup()
{
	// initialize the digital pin as an output.
	Serial.begin(9600);
	for (int c = 0; c < COLS; c++)
		pinMode(col[c], OUTPUT);
	for (int r = 0; r < ROWS; r++)
		pinMode(row[r], INPUT_PULLUP);

	Mouse.begin();
	trackpoint.reset();
	trackpoint.setSensitivityFactor(0x80);
	trackpoint.setStreamMode();
}

// This function will take keypresses passed to it (in the form of a char, for no particular reason)
// and add them to set of six keys that will be passed to the computer when Keyboard.send_now() is called.

// Basically, this collects the currently pressed keys and stores them until they can be passed to the computer.
void setKey(int keypress)
{
	/* Serial.println(keypress); */
	/* Serial.println(mouse); */

	// Look for unused keys in the buffer
	int i, j, m;
	for (i = 0; key[i] != 0; i++);
	for (j = 0; mod[j] != 0; j++);

	mouse = 0;
	// Catch Modifiers
	switch (keypress) {
	case 'L':
		ispress[0] = 1;
		break;
	case 'M':
		ispress[1] = 1;
		break;
	case 'R':
		ispress[2] = 1;
		break;
	case KEY_LEFT_CTRL:
	case KEY_LEFT_ALT:
	case KEY_LEFT_SHIFT:
	case KEY_LEFT_GUI:
		mod[j] = keypress;
		break;
	default:
		key[i] = keypress;
		break;
	}

	if (holdKey('^')) // Prevent setting layer key into set_key or set_modifier
		return;

	for (j = 0, m = 0; j < LEN(mod); j++)
		m |= mod[j];

	// Hold keypresses in buffer
	Keyboard.set_modifier(m);
	Keyboard.set_key1(key[0]);
	Keyboard.set_key2(key[1]);
	Keyboard.set_key3(key[2]);
	Keyboard.set_key4(key[3]);
	Keyboard.set_key5(key[4]);
	Keyboard.set_key6(key[5]);
}

// This method sends the depressed keys and clears the buffer.
void sendKey()
{
	/* if (holdKey('^')) */

	Keyboard.send_now();
	Mouse.set_buttons(ispress[0], ispress[1], ispress[2]);

	for (int i = 0; i < LEN(key); i++) key[i] = 0;
	for (int i = 0; i < LEN(mod); i++) mod[i] = 0;
	for (int i = 0; i < LEN(ispress); i++) ispress[i] = 0;

	Keyboard.set_modifier(mod[0]);
	Keyboard.set_key1(key[0]);
	Keyboard.set_key2(key[1]);
	Keyboard.set_key3(key[2]);
	Keyboard.set_key4(key[3]);
	Keyboard.set_key5(key[4]);
	Keyboard.set_key6(key[5]);
}

// Detects when a key is held down, returns true if held down, false if not
bool holdKey(int keypress)
{
	for (int i = 0; i < LEN(key); i++)
		if (key[i] == keypress)
			return true;
	return false;
}

// Calling this function will cycle to the next layer
void cycleLayer()
{
	if (currLayer == (LAYERS - 1)) // Reached maximum layer, going back to first layer
		currLayer = 0;
	else
		currLayer++; // Increments to the next layer
}

// Toggles between two layers, the curret layer and desired layer
void toggleLayer(int keyHeld, int desLayer)
{
	if (holdKey(keyHeld)) {
		prevLayer = currLayer; // Saves previous layer
		currLayer = desLayer; // Desired layer
	} else
		currLayer = prevLayer; // Returns to previous layer
}

// Macro sequence
void ctrlAltDel()
{
	// Using CTRL+ALT+KEYPAD_0 as example
	setKey(KEYPAD_0);
	setKey(176);
	setKey(177);

	sendKey();
}

// Goes to desired layer when keyHeld is pressed, returns to previous layer when released
void holdLayer(int keyHeld, int desLayer){
	if (holdKey(keyHeld)) {
		if (!toggleBind) { // Saves the previous layer, using boolean to prevent updating prevLayer more than once
			prevLayer = currLayer;
			toggleBind = 1;
		}
		currLayer = desLayer; // Desire layer
	} else {
		if (toggleBind)
			toggleBind = !toggleBind; // Resets boolean
		currLayer = prevLayer; // Returns to previous layer
	}
}

void sendButtonState(byte state) {
	static const char hidStates[] = {MOUSE_LEFT, MOUSE_RIGHT};

	for (byte i = 0; i < LEN(hidStates); i++) {
		byte hidState = hidStates[i];
		if (state & (1 << i)) {
			Mouse.press(hidState);
		} else if (Mouse.isPressed(hidState)) {
			Mouse.release(hidState);
		}
	}
}

void loop() {

	TrackPoint::DataReport d = trackpoint.readData();
	if (d.state & (1 << 2)) { // middle button down => scroll
		Mouse.move(0, 0, d.x);
	} else {
		Mouse.move(-d.y, -d.x, 0);
		sendButtonState(d.state);
	}

	/* /1* for (byte i = 0; i < LEN(mouseStates); i += 2) *1/ */
	/* for (int c = 4; c < 7; c++) { */
	/* 	digitalWrite(col[c], HIGH); */
	/* 	if (digitalRead(row[4])) { */
	/* 		if (!ispress[0]) { */
	/* 			Mouse.press(mouseStates[1]); */
	/* 			ispress[0] = 1; */
	/* 		} */
	/* 	} else if (ispress[0]) { */
	/* 		Mouse.release(mouseStates[1]); */
	/* 		ispress[0] = 0; */
	/* 	} */
	/* 	digitalWrite(col[c], LOW); */
	/* } */

	for (int c = 0; c < COLS; c++) {
		digitalWrite(col[c], HIGH);
		for (int r = 0; r < ROWS; r++)
			if (digitalRead(row[r]))
				setKey(layout[currLayer][r][c]);
		digitalWrite(col[c], LOW);
	}

	// setKey('a');
	// holdLayer('^', desiredLayer); // If the fn layer key is held, it changes the layer to the desired layer, when released, returns to previous layer

	// Now that all of the keys have been polled it is time to send them out!
	sendKey();
	delay(5);

}
