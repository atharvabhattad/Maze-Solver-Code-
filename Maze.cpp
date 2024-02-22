#define fthold 5 
#define rthold 7 
#define lthold 7

// Threshold value in front direction // Threshold value in right direction

const int t = 325;

// Threshold vlaue in left direction

int tfr =2750; //forward over right.

int timefr; //Dynamically set time.. for which it moves forward, when it chooses forward over left

// Time alotted for taking 90 degrees for 9V! // Initial Time for which it moves forward when it chooses


int tlbef=440;// Time for which it moves forward before taking left turn 
int tlaf=1150;// Tise for which it moves forward after taking left turn.
int nf=0; // Number of times it chooses straight over left.
int nlr=0; // Number of times it takes right turn.
bool found= false; // If its true, it indicates that the bot has reached theend!
int dir[100];// Array for storing the path of the bot. 
int i=-1;// For the indices of the dir array.
int j=0;// Implies the number of intersections bot passed through.

// Front US sensor. 
const int trigPinf= 2; 
const int echoPinf = 3;

// Right US sensor.

const int trigPinr= 8; 
const int echoPinr= 9;

// Left US sensor.

const int trigPinl=12;
const int echoPinl=11; 
//Booleans for recognising the walls. True if resp sensor distance is less than the resp threshold vlaue.

bool fsensor; // For the front IR sensor.
bool rsensor; // For the right US sensor..
bool lsensor; // For the left US sensor.
//Sorts and returns the median value of a five element array.
float middleval(float arr[])
{
	for(int p=0;p<4;p++)
	{
		for(int q=0;q<4;q++)
		{
			if(arr[q]>arr[q+1])
			{
                int temp = arr[q];
                arr[q]= arr[q+1];
                arr[q+1] =temp;
            }
        }
    }
	return arr[2]; // Median value.
}

void gofront()//Moves the bot forward
{
//Moves forward while adjusting the path

    float ldist1= leftdist(); 
    float lconst= ldist1;
    while(ldist1<=5) //Should turn a little to the right
    {
        digitalWrite(4,HIGH);
        digitalWrite(6,LOW); 
        digitalWrite(7,LOW);
        digitalWrite(13,LOW); 
        
		delay(t/65);
        ldist1=leftdist();
        if(abs(lconst - ldist1)>-0.8||(ldist1>=3.6)){break;}
    }
    float rdist1= rightdist();
    float rconst= rdist1;
    
    while(rdist1<=5.4) // Should turn a little to its left
    {
        digitalWrite(4,LOW);
        digitalWrite(6,LOW); 
        digitalWrite(7,HIGH);
        digitalWrite(13,LOW); 
		
		delay(t/65);

        rdist1=rightdist();
        if(abs(rconst-rdist1)>=0.9){break;}
    }//Removing a part of code(snippets from pg5 and 6)
    digitalWrite(4,HIGH);
    digitalWrite(6,LOW);
    digitalWrite(7,HIGH);
    digitalWrite(13,LOW);
}

float frontdist()
{
    float gapf;
	float ticktockf;
    digitalWrite(trigPinf, LOW);
    delayMicroseconds(2); 
	digitalWrite(trigPinf, HIGH); 
	delayMicroseconds(10);
    digitalWrite(trigPinf,LOW);
    
	ticktockf= pulseIn(echoPinf,HIGH); 
	gapf=ticktockf*0.0344/2;
    
	return gapf;
    }
    
	float rightdist()
{
    float gapr; 
    float ticktockr; 
	digitalWrite(trigPinr,LOW); 
	delayMicroseconds(2);
    digitalWrite(trigPinr,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinr,LOW);
    
	ticktockr = pulseIn(echoPinr,HIGH); 
	gapr=ticktockr*0.0344/2;
    
	return gapr;
}

float leftdist(){
	float gapl; 
	float ticktockl;
    digitalWrite(trigPinl,LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinl,HIGH); 
	delayMicroseconds(10); 
	digitalWrite(trigPinl, LOW);

    ticktockl= pulseIn(echoPinl, HIGH);
    gapl=ticktockl* 0.0334/2;
	return gapl;
}


void reduce(int dir[], int &pt) //Reduces the size of path
{
    int i=pt;
    if(i>=2)
    {
        if((dir[i-1]==3)&&(dir[i-2]==2)&&(dir[i]==1))//RUL=U
        {
            dir[i-2]=3;
            pt=pt-2; 
        }
        else if((dir[i-1]==3)&&(dir[i-2]==1)&&(dir[i]==1))//LUL=F
        {
            dir[i-2]=0;
            pt = pt -2;
        } 
        else if((dir[i-1]==3)&&(dir[i-2]==1)&&(dir[i]==2))//LUR=U
        {
            dir[i-2]=3;
            pt = pt -2;
        }
        else if((dir[i-1]==3)&&(dir[i-2]==0)&&(dir[i]==0))//FUF=U
        {
            dir[i-2]=3; 
            pt= pt -2;
        }

        else if((dir[i-1]==3)&&(dir[i-2]==0)&&(dir[i]==1))//FUL==R
        {
            dir[i-2]=2;
            pt=pt-2;
        }

        else if((dir[i-1]==3)&&(dir[i-2]==1)&& (dir[i]==0))//
        {
            dir[i-2]=2; 
            pt=pt-2;
        }
        return;
    }
}




void stopit()//Stops the bot
{
    digitalWrite(4,LOW);
    digitalWrite(6,LOW); 
    digitalWrite(7,LOW);
    digitalWrite(13,LOW); 
}
 void frontturn()//Move forward according to the shortest path(At some intersection)
{
    for(int n=1;n<=8;n++) 
    {gofront(); delay((timefr)/8);}

    digitalWrite(4,LOW);
    digitalWrite(6,LOW); 
    digitalWrite(7,LOW);
    digitalWrite(13,LOW); 
    delay(1000);
}
    
void leftturn() //Turns left according to shortest path 
{
    stopit(); delay(1000);
    float prevfdist = frontdist();
//while loop in code commented here idky
    for (int n=1;n<=5;n++) {gofront(); delay(260);}

    digitalWrite(4,HIGH);
    digitalWrite(6,LOW); 
    digitalWrite(7,LOW);
    digitalWrite(13,HIGH);  
    delay(t);
    
// gofront(); delay(2400); commented here, dont know why
    float prevfrdist = frontdist();

    while( abs(frontdist()-prevfrdist)<=18)  
    //for loop commented here, dont know why
	{gofront(); delay(300);}

    digitalWrite(4,LOW);
    digitalWrite(6,LOW); 
    digitalWrite(7,LOW);
    digitalWrite(13,LOW);
    delay(1000);
}




void setup() 
{
//US pins setup
    pinMode (trigPinf, OUTPUT);
    pinMode (echoPinf, INPUT);
    pinMode (trigPinr, OUTPUT);
    pinMode (echoPinr, INPUT);
    pinMode (trigPinl,OUTPUT);
    pinMode (echoPinl, INPUT);

// Motor pins.
    pinMode(4,OUTPUT);
    pinMode(6,OUTPUT); 
    pinMode(7,OUTPUT);
    pinMode(13,OUTPUT);
    pinMode(5,OUTPUT);
    pinMode(10,OUTPUT);
	    
    Serial.begin(9600); 
    dir[0]=0;
 // initial direction..?
    }
void loop() 
{
  analogWrite(5, 255);
  analogWrite(10, 255);
    if(nlr==4)
    {
        found=true; 
// Reached the end.

    for(int i=0;i<=2;i++) {Serial.print(dir[i]);} 
	i=-1;j=0; nlr=0; 
// Back to start again..

// Stops the bot for 30 seconds after reaching the end.

    digitalWrite(4,LOW);
    digitalWrite(6,LOW); 
    digitalWrite(7,LOW);
    digitalWrite(13,LOW); 
    delay(10000);
}

float fdist;
float rdist;
float ldist;
// front, right, and left distances.
 float fduration; float rduration; float lduration; 
// front, right, and left travel time in echopin..

float fdur[5]; float rdur[5]; float ldur[5];
 // Arrays which store the values of five durations.
 // We only take median to reduce error
float ldista[5];

// For the front US sensor..
 for(int i=0;i<=4;i++)
{
    digitalWrite(trigPinf, LOW); 
	delayMicroseconds(5);

// Clearing the trigpin.

    digitalWrite(trigPinf,HIGH); 
// Setting the trigpin HIGH for 10 microseconds..sends some 8cycle sonics.

    delayMicroseconds(10);
    digitalWrite(trigPinf, LOW);

    fdur[i] = pulseIn(echoPinf,HIGH); //returns the time the wave travelled
}
fduration = middleval(fdur);
fdist = fduration*0.0344/2;

Serial.print("frontdistance: ");
Serial.println(fdist);
// for the right us sensor...

for(int i=0;i<=4;i++)
{
digitalWrite(trigPinr, LOW); 
delayMicroseconds(5);
digitalWrite(trigPinr, HIGH);
delayMicroseconds(10);
digitalWrite(trigPinr,LOW);

rdur[i]= pulseIn(echoPinr,HIGH);
}


rduration= middleval(rdur); 
rdist= rduration*0.0344/2;
/*Serial.print("rightdistance: ");
Serial.println(rdist);*/

// Distance of the wall to its right.

// for the left US sensor..... 
for(int i=0;i<=4;i++)
{
digitalWrite(trigPinl,LOW); 
delayMicroseconds(5); 
digitalWrite(trigPinl,HIGH);
delayMicroseconds(10);
digitalWrite(trigPinl,LOW);
ldur[i]= pulseIn(echoPinl,HIGH);
}

lduration= middleval(ldur); 
ldist= lduration*0.0344/2;

/*Serial.print("leftdistance: ");

// Distance of the wall to its left side

Serial.println(ldist);"*/

if((fdist>=125)||(rdist>= 150)||(ldist>=400)) //check dimensions of maze and change accordingly the values
{return;} // Cancelling out any error values...goes back to void loop().
 
fsensor = false; rsensor = false; lsensor = false;

// Setting up the booleans

  if(rdist<=rthold) {rsensor= true;} 

  if(ldist<=lthold) {lsensor= true;}

  if(fdist<=fthold) {fsensor= true;}


// Right wall following algo

// If right is closed- 
if((rsensor==true))
{                    //for u turn
if((lsensor == true)&&(fsensor==true))
  {
    j = j+1;
    i = i+1;
    dir[i] = 3;
    reduce(dir,i);

    digitalWrite(4,HIGH);
    digitalWrite(6,LOW);
    digitalWrite(7,LOW);
    digitalWrite(13,HIGH);

    delay(2*t);
	}

// If Front is open..

else if(fsensor==false)
{
if((lsensor==false)&&(frontdist()>=40)) // If both front and left are open..
{
    i = i+1;
    j = j+1;
    if((found==true)&&(dir[i]!=0)) // After reaching the end... checks the s
 {
    leftturn();return;
 }
else
    {
    if(found==false){
	dir[i] =0;
    reduce(dir,i);}
// moving forward over left... reduce(dir, 1);

/*Serial.print("for the ith turn.."); Serial.print(" "); Serial.print(1); 
Serial.print(" the value is "); Serial.print(1); Serial.print("and the dir is.."); 
Serial.println(dir[1]);*/

    timefr = tfr+ 65*nf;   
    nf=nf+1;
     
    stopit();delay(500);

    for(int g=1;g<=10;g++){gofront(); delay(timefr/10);}

    stopit();delay(500);
 }
}
else {gofront(); delay(300);} // Else moving forward.. only front is open.
}

//for a left turn

    else
    {
	i = i+1;
    j = j+1;

    dir[i] = 2; 
    reduce(dir,i);

    float prevfdist=frontdist();

    while( abs(frontdist()-prevfdist)<=(prevfdist/2)-2) 
{gofront(); delay(300);if(frontdist()<=4.5){break;}}

  digitalWrite(4,LOW);

  digitalWrite(6,HIGH);

  digitalWrite(7,HIGH); 

  digitalWrite(13,LOW);

  delay(t);

  float prevfrdist = frontdist();

  while( abs(frontdist()-prevfrdist)<=15.2) 
  {gofront(); delay(300); if(frontdist()<=4.5){break;}}
}
}


else

//for a RIGHT turn..
{ i = i+1;
  j = j+1;
if((found==true)&&(dir[i]!=1))
{

if((dir[i]==2)&&(leftdist>=10)) {leftturn(); return;}

    else if((dir[i] == 0)&&(fsensor == false))

{frontturn();return;}}

    else
    {dir[i]=1;
// right turn..
    nlr=nlr+1;
    reduce(dir, i); //calling reduce function to shorten the path dynamically. If path is not yet completed

    {gofront(); delay(tlbef);}

        digitalWrite(4, HIGH); // takes a right turn..
        digitalWrite(6, LOW); 
        digitalWrite(7, LOW); 
        digitalWrite(13, LOW); 
        delay(2*t);
        for(int n=1;n<=8;n++) { gofront(); delay(tlaf/8);}

        stopit(); delay(1000);
    }
}
delay(320);
}
