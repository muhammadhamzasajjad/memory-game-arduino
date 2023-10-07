 #include <Adafruit_RGBLCDShield.h>
 #include <EEPROM.h>

#define WAITING 1
#define CREATING_SEQUENCE 2
#define READING_INPUT 3
#define CHECKING_INPUT 4

#define MAX_N 16  //game variables min and max values
#define MAX_STORY_N 8
#define MIN_N 4
#define MAX_M 4
#define MIN_M 2
#define MAX_D 5
#define MIN_D 1
#define MIN_T 1
#define MAX_T 4 
#define RIGHT 0   //arrows code
#define LEFT 1
#define UP 2
#define DOWN 3
#define UP_DOWN 4

#define NO_MODE -1    //main menu options
#define PRACTICE 0
#define STORY 1
#define TOP_SCORES 2
#define SHOP 3
#define REWARDS 4
#define INIT 5


#define DIFF_INPUT 0  //story mode and practice mode states
#define PLAYING 1
#define SAVING_SCORE 2

#define GO_BACK 0
#define SET_N 1
#define SET_M 2
#define SET_D 3
#define SET_T 4

#define NO_RESULT 0 //possible outcomes of play()
#define LOST 1
#define WON 2

#define EEPROM_SCORE 100
#define EEPROM_COINS 150
#define EEPROM_TOTAL_SCORE 140

#define SET_COLOR 1

#define TOTAL_REWARDS 16
#define REWARD_RATE 300  //every 300 score you unlock a new reward


Adafruit_RGBLCDShield lcd=Adafruit_RGBLCDShield();

uint8_t gameState=WAITING;
unsigned long latestMillis=0;  
uint8_t S[MAX_N];
uint8_t S_INPUT[MAX_N];
uint8_t M=MIN_M;
uint8_t N=MIN_N;
uint8_t D=MIN_D;
int currentD=D;
uint8_t T=2;
uint8_t readIndex=0;
uint16_t score=0;
int8_t mode=NO_MODE;
uint8_t selectedMode=PRACTICE;
uint8_t selectedSet=SET_N;
uint8_t storyState=DIFF_INPUT;
uint16_t totalCoins=0;
int8_t selectedopt=SET_COLOR;
uint8_t currentColor=0;
uint8_t selectedColor=0;
uint16_t totalScore=0;
uint8_t rewardIndex=0;
uint8_t rewardsUnlocked=0;

//byte *arrows[]={rightArrow,leftArrow,upArrow,downArrow};
uint8_t lastbtn=0;
bool printed=false;
bool doblink=false;
int8_t practiceState=DIFF_INPUT;
uint8_t scoreIndex=0;

byte upDownArrow[] = {
    B00100,
    B01110,
    B11111,
    B00000,
    B00000,
    B11111,
    B01110,
    B00100
};


void setup() {
  lcd.begin(16,2);
  //Serial.begin(9600);
  lcd.setBacklight(5);
  lcd.print("Welcome");
  delay(2000);
  currentColor=EEPROM.read(EEPROM_COINS+10);
  selectedColor=currentColor;
  EEPROM.get(EEPROM_COINS,totalCoins);
  EEPROM.get(EEPROM_TOTAL_SCORE,totalScore);
  rewardsUnlocked=totalScore/REWARD_RATE;
  
  lcd.setBacklight(currentColor);
  createChars();
  latestMillis=millis();
  randomSeed(analogRead(0));
  lcd.clear();
}

void loop() {
  switch(mode){
      case NO_MODE:
        noMode();
      break;
      case STORY:
        story();
        break;
      case PRACTICE:
        practice();
        break;
      case TOP_SCORES:
        showHighScores();
        break;
      case SHOP:
        shop();
        break;
      case INIT:
        Init();
        break;
      case REWARDS:
        rewards();
        break;
  }
}

//initializes the values in EEPROM i.e. coins colors and top socres
void Init(){
  uint8_t btn=lcd.readButtons();
  //if within two seconds the user does not press the select button then return to main menu
  if(millis()-latestMillis>2000){
    lcd.clear();
    mode=NO_MODE;
  }
  else if(!(btn&BUTTON_SELECT)&&(lastbtn&BUTTON_SELECT)){
    lcd.setCursor(0,0); lcd.print("resetting");

    //resetting high score
    for(int i=0;i<5;i++){
        EEPROM.update(i*5+EEPROM_SCORE,0);
        EEPROM.update(i*5+EEPROM_SCORE+1,0);
    }

    //resettting totalcoins
    EEPROM.update(EEPROM_COINS,0);
    EEPROM.update(EEPROM_COINS+1,0);

    //resetting owned colors and current color
    for(int i=EEPROM_COINS+3;i<=EEPROM_COINS+10;i++){
      EEPROM.update(i,0);
    }
    //resetting totalScore
    EEPROM.update(EEPROM_TOTAL_SCORE,0);
    EEPROM.update(EEPROM_TOTAL_SCORE+1,0);
    
    currentColor=EEPROM.read(EEPROM_COINS+10);
    selectedColor=currentColor;
    EEPROM.get(EEPROM_COINS,totalCoins);
    EEPROM.get(EEPROM_TOTAL_SCORE,totalScore);
    rewardsUnlocked=totalScore/REWARD_RATE;
    
    lcd.setBacklight(currentColor);
    lcd.clear();
    lcd.setCursor(0,0); lcd.print("Resetting done.");
    delay(1300);
    lcd.clear();
    mode=NO_MODE;
    selectedMode=PRACTICE;
  }
  else{
    //show this message as the Init option is selected for two seconds (or until user presses select)
    lcd.setCursor(0,0);
    lcd.print("Select to reset");
  }

  lastbtn=btn;
}

void createChars(){
   byte upArrow[] = {
      B00000,
      B00000,
      B00100,
      B01110,
      B11111,
      B00000,
      B00000,
      B00000
  };
  byte downArrow[] = {
      B00000,
      B00000,
      B11111,
      B01110,
      B00100,
      B00000,
      B00000,
      B00000
  };
  byte leftArrow[] = {
      B00000,
      B00010,
      B00110,
      B01110,
      B00110,
      B00010,
      B00000,
      B00000
  };
  byte rightArrow[] = {
      B00000,
      B01000,
      B01100,
      B01110,
      B01100,
      B01000,
      B00000,
      B00000
  };
  

  //creating chars and giving them the associated ids
  lcd.createChar(RIGHT,rightArrow);
  lcd.createChar(LEFT,leftArrow);
  lcd.createChar(UP,upArrow);
  lcd.createChar(DOWN,downArrow);
  lcd.createChar(UP_DOWN,upDownArrow);
}

//show the menu and if select button is pressed passes to the selected mode
void noMode(){
  uint8_t btn=lcd.readButtons();
  
  String opt="";
  switch(selectedMode){
    case PRACTICE:
      opt="Practice";
      break;
    case STORY:
      opt="Story ";
      break;
    case TOP_SCORES:
      opt="TOP Scores";
      break;
    case SHOP:
      opt="Shop";
      break;
    case REWARDS:
      opt="REWARDS";
      break;
    case INIT:
      opt="Initialize";
      break;
    
  }
      
  printMenuOpt(opt,0);
  lcd.setCursor(15-String(totalCoins).length(),1);
  lcd.print("$"+(String)totalCoins);
  
  //if right or left buttons are released switch to the next/previous menu option
  if((!(btn&BUTTON_RIGHT)&&(lastbtn&BUTTON_RIGHT))){
    selectedMode=(selectedMode+1)%6;
    clearLine(true);
  }
  
  if((!(btn&BUTTON_LEFT)&&(lastbtn&BUTTON_LEFT))){
    selectedMode=(selectedMode+5)%6;
    clearLine(true);
  }
  
  //switch to the selected option when selected button is released
  if(!(btn&BUTTON_SELECT)&&(lastbtn&BUTTON_SELECT)){
    mode=selectedMode;
    lcd.clear();
    if(selectedMode==PRACTICE){
      practiceInit();
    }
    else if(selectedMode==STORY){
      storyInit();
    }
    else if(selectedMode==TOP_SCORES){
      topScoresInit();
    }
    else if(selectedMode==REWARDS){
      rewardIndex=rewardsUnlocked-1;
    }
    latestMillis=millis();
  }
  
  lastbtn=btn;
}

int8_t rank=-1;

//funtions to initialize the different options from the menu
void storyInit(){
  M=MIN_M; N=MIN_N;
  score=0;
  D=1; T=2;
  gameState=CREATING_SEQUENCE;
  storyState=DIFF_INPUT;
  selectedSet=SET_N;
}

void practiceInit(){
  practiceState=DIFF_INPUT;
  selectedSet=SET_N;
}

void topScoresInit(){
  latestMillis=millis()-500;
  scoreIndex=0;
  
}

//executed when practice mode is selected
void practice(){
  //switch(practiceState){
    
    //case DIFF_INPUT:
  if(practiceState==DIFF_INPUT){
    
    int8_t inputOutcome=inputDifficulty(3);
    //if the user has selected the difficulty then pass to the playing mode
    if(inputOutcome==1){
      gameState=CREATING_SEQUENCE;
      practiceState=PLAYING;
      currentD=D*1000;
    }
    //break;
  }
  else{
  //case PLAYING:
    int8_t outcome=play();
    if(outcome){
      lcd.clear();
      if(outcome==WON){
        lcd.setBacklight(2); lcd.setCursor(0,0); lcd.print("YOU WON");
        
      }
      else{
        lcd.setBacklight(1); lcd.setCursor(0,0); lcd.print("YOU LOST");
        
      }
      delay(2000);
      lcd.setBacklight(currentColor);
      practiceState=DIFF_INPUT;
      lcd.clear();
    }
    
  }
}

int8_t storyRank=0;

uint8_t selectedAlias=0;

//executed when story mode is selected
void story(){

  if(storyState==DIFF_INPUT){
    int8_t inputOutcome=inputDifficulty(4);
    //if the user has selected a difficulty level then pass to the next level
    if(inputOutcome==1){
      gameState=CREATING_SEQUENCE;
      storyState=PLAYING;
      currentD=D*1000;
    }
    return;
  }
  if(storyState==SAVING_SCORE){
    if(saveScore()){
      mode=NO_MODE;
    }
    return;
  }
  if(storyState==PLAYING){
    //playing
    uint8_t outcome=play();
    if(outcome){
      lcd.setCursor(0,0);
      decrementCurrentD();
      //if you lose or reach the last level then the game is over.
      if(outcome==LOST||(M==MAX_M && N==MAX_STORY_N)){
        if(M==MAX_M && N==MAX_STORY_N && outcome==WON){
          score+=calculateScore();
          
        }
        //saving total coins and totalscore
        totalCoins+=score*2;
        EEPROM.put(EEPROM_COINS,totalCoins);
        rank=findRank(score);
        totalScore+=score;
        EEPROM.put(EEPROM_TOTAL_SCORE,totalScore);

        //if a rank in top score is found then save the score
        if(rank!=-1){
          storyState=SAVING_SCORE;
          
          selectedAlias=0;
          if(M==MAX_M && N==MAX_STORY_N && outcome==WON){
            lcd.print("Game ends"); lcd.setBacklight(2);
          }
          else{
            lcd.print("Game over"); lcd.setBacklight(1);
          }
          
          lcd.setCursor(0,0);
          delay(1500);
          lcd.setBacklight(2);
          
          //you win a new reward as the total rewards get past a multiple of REWARD_RATE
          if(totalScore-score>0&&totalScore/REWARD_RATE>(totalScore-score)/REWARD_RATE&&totalScore/REWARD_RATE<=TOTAL_REWARDS){
            lcd.print("new Reward");
            rewardsUnlocked=totalScore/REWARD_RATE;
            createReward(rewardsUnlocked-1);
            lcd.setCursor(11,0); lcd.write(7);
            lcd.setCursor(0,0);
            delay(2000);
            lcd.clear();
          }
          
          //showing the rank
          if(rank==1){
            lcd.print("TOP scorer!");
          }
          else if(rank==2){
            lcd.print((String)rank+"nd Top score");
          }
          else if(rank==3){
            lcd.print((String)rank+"rd Top score");
          }
          else{
            lcd.print((String)rank+"th Top score");
          }
        }
        else{
          lcd.setBacklight(1); lcd.print("Game over");
          mode=NO_MODE;
        }
        
        lcd.setCursor(0,1); lcd.print("Score "+String (score));
        delay(1500);
        
        N=MIN_N;
        M=MIN_M;
        
      }
      else if(outcome==WON){
        lcd.setBacklight(2); lcd.print("Next Level");
        if(N==MAX_STORY_N){
          gameState=CREATING_SEQUENCE;
          N=MIN_N; M++;
          
        }
        else{
          score+=calculateScore();
          N++;
          gameState=CREATING_SEQUENCE;
          
        }
        delay(1000);
      }
      lcd.clear(); lcd.setBacklight(currentColor);
    }
    return;
  }
}

uint16_t calculateScore(){
  return M*N-D-2;
}

void decrementCurrentD(){
  if(currentD>300){
    int d=ceil(currentD/1000.0)*1000*10/100;
    currentD-=d;
  }
}

void showHighScores(){
  uint8_t btn=lcd.readButtons();

  //if the last highscore was show for more than a second then show the next highest score
  if(millis()-latestMillis>1000){
    if(scoreIndex==5){
      scoreIndex=0;
    }
    scoreIndex++;
    
    clearLine(true);
    lcd.setCursor(0,0);
    lcd.print(getTopScore(scoreIndex));
    latestMillis=millis();
  }
  
  lcd.setCursor(0,0);
  
  lcd.setCursor(0,1);
  lcd.print("Select to exit");

  //go back when select is released
  if((lastbtn&BUTTON_SELECT)&&!(btn&BUTTON_SELECT)){
    lcd.clear();
    mode=NO_MODE;
  }

  lastbtn=btn;
}

void shop(){

  uint8_t btn=lcd.readButtons();
  if(!(btn&BUTTON_RIGHT)&&(lastbtn&BUTTON_RIGHT)){
    selectedopt=(selectedopt+1)%(2);
    lcd.clear();
  }
  if(!(btn&BUTTON_LEFT)&&(lastbtn&BUTTON_LEFT)){
    selectedopt=(selectedopt+1)%(2);
    lcd.clear();
  }
  
  String opt="";
  switch(selectedopt){
    case SET_COLOR:
      opt="Color";
      incrementDecrement(btn,&selectedColor,7,0);
      lcd.setBacklight(selectedColor);
      if((lastbtn&BUTTON_SELECT)&&!(btn&BUTTON_SELECT)){
        clearLine(false);
        lcd.setCursor(0,1);
        
        if(EEPROM.read(EEPROM_COINS+2+selectedColor)!=0){
          currentColor=selectedColor;
          lcd.print("Color selected");
          
        }
        else if(totalCoins>=selectedColor*150){
          currentColor=selectedColor;
          totalCoins-=selectedColor*150;
          EEPROM.put(EEPROM_COINS,totalCoins);
          EEPROM.update(EEPROM_COINS+2+selectedColor,1);
          lcd.print("Color selected");
        }
        else{
          lcd.print("Need $"+String(selectedColor*150-totalCoins)+" more");
        }
        delay(1000);
        clearLine(false);
      }
      break;
    case GO_BACK:
      lcd.setBacklight(currentColor);
      opt="Go back";
      if((lastbtn&BUTTON_SELECT)&&!(btn&BUTTON_SELECT)){
        mode=NO_MODE;
        
        selectedopt=SET_COLOR;
        //selectedColor=currentColor;
        lcd.clear();
        lastbtn=btn;

        if(EEPROM.read(EEPROM_COINS+10)!=currentColor){
          EEPROM.update(EEPROM_COINS+10,currentColor);
        }
        
        return;
      }
      break;
  }

  if(selectedopt!=GO_BACK){
    lcd.setCursor(1,0); lcd.write(UP_DOWN);
    
    lcd.setCursor(15-String(totalCoins).length(),1); lcd.print("$"+(String)totalCoins);
    
    if(currentColor==selectedColor){
      lcd.setCursor(0,1);
      lcd.print("Selected ");
    }
    else{
      lcd.setCursor(0,1);
      if(EEPROM.read(EEPROM_COINS+2+selectedColor)!=0){
        lcd.print("Owned   ");
      }
      else{
        lcd.print("For $"+String(selectedColor*150)+"   ");
      }
      
      
    }
  }
  
  printMenuOpt(opt,0);
  
  lastbtn=btn;
}

//displays the rewards that the user has unlocked
void rewards(){
  uint8_t btn=lcd.readButtons();
  if(rewardsUnlocked==0){
    lcd.setCursor(0,0);
    lcd.print("NO rewards");
    
  }
  else{
    createReward(rewardIndex);
    incrementDecrement(btn,&rewardIndex,rewardsUnlocked-1,0);
    //printMenuOpt("",0);
    lcd.setCursor(0,0);
    lcd.write(UP_DOWN);
    createReward(rewardIndex);
    lcd.setCursor(7,0);
    lcd.write(7);
    lcd.setCursor(13,0);
    lcd.print(String(rewardIndex+1)+" ");
  }
  lcd.setCursor(0,1);
  lcd.print("Select to exit");
  if((lastbtn&BUTTON_SELECT)&&!(btn&BUTTON_SELECT)){
    lcd.clear();
    mode=NO_MODE;
  }
  lastbtn=btn;
}

//the function prints a menu option in the center of the top row of the lcd
void printMenuOpt(String opt,uint8_t row){
  lcd.setCursor(0,row); lcd.write(LEFT);
  lcd.setCursor(15,row); lcd.write(RIGHT);
  //between the arrows printing the opt in center of the screen
  lcd.setCursor(8-opt.length()/2,row); lcd.print(opt);
}

//clears the first line if true is passed otherwised clears the second line
void clearLine(bool lineOne){
  if(lineOne){
    lcd.setCursor(0,0);
  }
  else{
    lcd.setCursor(0,1);
  }
  lcd.print("                ");
}

//runs a game play and returns NO_RESULT WON OR LOST according to the outcome of the game
int8_t play(){
  switch(gameState){
    case CREATING_SEQUENCE:
      generateRandomSequence();
      //delay(2000);
      clearLine(false);
      gameState=READING_INPUT;
      latestMillis=millis();
      
      return NO_RESULT;
      break;
    case CHECKING_INPUT:
      clearLine(false); lcd.setCursor(0,1); printSequence();
      
      if(sequencesAreEqual()){
        for(uint8_t i=0;i<5;i++){
          lcd.setBacklight(2);
          delay(100);
          lcd.setBacklight(0);
          delay(100);
        }
        lcd.setBacklight(currentColor);
        //delay(1000);
        lcd.clear();
        return WON;
      }
      else{
        lcd.setBacklight(1);
        delay(2000);
        //delay the display until the select button is kept pressed
        while(lcd.readButtons()&BUTTON_SELECT){}
        lastbtn=lcd.readButtons();
        lcd.setBacklight(currentColor); lcd.clear();
        return LOST;
      }
      
      break;
    case READING_INPUT:
      int readBtn=readReleasedButton();
      
      //countdown
      int tenths=((T*1000-(millis()-latestMillis))/100)%10;
      int sec=(T*1000-(millis()-latestMillis))/1000;
      
      lcd.setCursor(10,1); lcd.print((String)sec+"."+(String)tenths);
      
      if(readBtn!=-1){
        latestMillis=millis();
        S_INPUT[readIndex]=readBtn;
        lcd.setCursor(readIndex,0);
        lcd.write(S_INPUT[readIndex]);
        readIndex++;
        
        if(readIndex>=N){
          readIndex=0;
          gameState=CHECKING_INPUT;
          delay(1000);
          lcd.setBacklight(currentColor);
        }
      }
      else{
        if(millis()-latestMillis>(T*1000)){
          lcd.clear();
          return LOST;
        }
      }
      
      break;
  }

  return NO_RESULT;
}

//reads top 5 scores from the eeprom and returns them in a string
String getTopScore(int8_t rank){
  String scores="";
  //uint8_t highScore=EEPROM.read((rank-1)*5+EEPROM_SCORE);
  uint16_t highScore;
  EEPROM.get((rank-1)*5+EEPROM_SCORE,highScore);
  if(highScore==0){
    scores+= (String)rank+")0 by --- ";
  }
  else{
    char c1=EEPROM.read((rank-1)*5+2+EEPROM_SCORE);
    char c2=EEPROM.read((rank-1)*5+3+EEPROM_SCORE);
    char c3=EEPROM.read((rank-1)*5+4+EEPROM_SCORE);
    scores+=(String)rank + ")"+(String)highScore+" by "+c1+c2+c3+" ";
  }

  return scores;
}


//allows the user to set the initial values of M,N,D and T
//returns 1 if select is pressed (the user is ready to play) otherwise -1
int8_t inputDifficulty(int8_t options){
  
    uint8_t btn=lcd.readButtons();
      if(!(btn&BUTTON_RIGHT)&&(lastbtn&BUTTON_RIGHT)){
        selectedSet=(selectedSet+1)%(options+1);
        lcd.clear();
      }
      if(!(btn&BUTTON_LEFT)&&(lastbtn&BUTTON_LEFT)){
        selectedSet=(selectedSet+options)%(options+1);
        lcd.clear();
      }
      
      lcd.setCursor(0,1);
      String opt="";
      switch(selectedSet){
        case SET_N:
          if(mode==STORY){
            incrementDecrement(btn,&N,MAX_STORY_N,MIN_N);
          }
          else{
            incrementDecrement(btn,&N,MAX_N,MIN_N);
          }
          
          opt=" LENGTH "+(String) N+" ";
          break;
        case SET_M:
          incrementDecrement(btn,&M,MAX_M,MIN_M);
          opt="Symbols "+(String) M;
          break;
        case SET_D:
          incrementDecrement(btn,&D,MAX_D,MIN_D);
          opt="D "+(String) D;
          break;
        case SET_T:
          incrementDecrement(btn,&T,MAX_T,MIN_T);
          opt="T "+(String) T;
          break;
        case GO_BACK:
          if((lastbtn&BUTTON_SELECT)&&!(btn&BUTTON_SELECT)){
            mode=NO_MODE;
            lcd.clear();
            //return -1;
          }
          opt="GO BACK";
          break;
      }
      if(selectedSet!=GO_BACK){
        lcd.setCursor(1,0);
        lcd.write(UP_DOWN);
        lcd.setCursor(0,1);
        lcd.print("select to play");
      }
      
      printMenuOpt(opt,0);

      if(!(btn&BUTTON_SELECT)&&(lastbtn&BUTTON_SELECT)){
        lcd.clear();
        lastbtn=btn;
        return 1;
      }
      
      lastbtn=btn;
      return -1;
}


void printSequence(){
  for(int i=0;i<N;i++){
    lcd.write(S[i]);
  }
}

//the function increments or decrements the value pointed by p depending on whether or not up or down buttons are pressed and released
void incrementDecrement(uint8_t btn,uint8_t *p,uint8_t maxp,uint8_t minp){
  //uint8_t btn=lcd.readButtons();
  
  if(!(btn&BUTTON_UP)&&(lastbtn&BUTTON_UP)){
    if(*p==maxp){
      *p=minp;
    }
    else{
       *p=*p+1;
    }
  }
  if(!(btn&BUTTON_DOWN)&&(lastbtn&BUTTON_DOWN)){
    if(*p==minp){
      *p=maxp;
    }
    else{
      *p=*p-1;
    }
  }

  
}



char alias[3]={'A','A','A'};

//allows the user to set the three aliases and to save them in the eeprom with the associated highscore
bool saveScore(){

  uint8_t btn=lcd.readButtons();
  if((!(btn&BUTTON_UP)&&(lastbtn&BUTTON_UP))||(!(btn&BUTTON_DOWN)&&(lastbtn&BUTTON_DOWN))){
    incrementDecrement(btn,&alias[selectedAlias],'Z','A');
    doblink=false;
    latestMillis=millis()-301;
  }
  
  if(!(btn&BUTTON_RIGHT)&&(lastbtn&BUTTON_RIGHT)){
    selectedAlias=(selectedAlias+1)%3;
  }
  if(!(btn&BUTTON_LEFT)&&(lastbtn&BUTTON_LEFT)){
    selectedAlias=(selectedAlias+2)%3;
  }

  if(!(btn&BUTTON_SELECT)&&(lastbtn&BUTTON_SELECT)){
    if(rank!=-1)
      writeScore(rank,score,alias[0],alias[1],alias[2]);
    lcd.clear();
    lastbtn=btn;
    return true;
  }
  
  if(millis()-latestMillis>200){
    char chars[3]={alias[0],alias[1],alias[2]};
    if(doblink){
      chars[selectedAlias]=' ';
    }
    lcd.setCursor(0,0);
    lcd.print(chars);
    doblink=!doblink;
    latestMillis=millis();
    lcd.setCursor(0,1);
    lcd.print("select to save");
  }
  //delay(1);
  lastbtn=btn;
  return false;
}

//writes the score an the alias in the rank position and shifts the scores, having lower ranks, down by one position
void writeScore(int8_t rank,int8_t score,char c1,char c2,char c3){
  for(int i=rank-1;i<5;i++){
    uint16_t scoreS;
    //EEPROM.read((i*4)+EEPROM_SCORE);
    EEPROM.get((i*5)+EEPROM_SCORE,scoreS);
    char c1S=EEPROM.read((i*5)+2+EEPROM_SCORE);
    char c2S=EEPROM.read((i*5)+3+EEPROM_SCORE);
    char c3S=EEPROM.read((i*5)+4+EEPROM_SCORE);
    if(score!=scoreS){
      //EEPROM.write((i*4)+EEPROM_SCORE,score);
      EEPROM.put(((i*5)+EEPROM_SCORE),score);
    }
    if(c1!=c1S){
      EEPROM.update((i*5)+EEPROM_SCORE+2,c1);
    }
    if(c2!=c2S){
      EEPROM.update((i*5)+EEPROM_SCORE+3,c2);
    }
    if(c3!=c3S){
      EEPROM.update((i*5)+EEPROM_SCORE+4,c3);
    }
    score=scoreS;c1=c1S;c2=c2S;c3=c3S;
  }
}

//returns true if S_INPUT and S are equal false otherwise
bool sequencesAreEqual(){
  for(int8_t i=0;i<N;i++){
    if(S[i]!=S_INPUT[i]){
      //lcd.setBacklight(1);
      return false;
    }
  }
  //lcd.setBacklight(2);
  return true;
}

//return released button among the possible sequence inputs if none is released then it returns -1
int8_t readReleasedButton(){
  uint8_t btn=lcd.readButtons();
  int rtn=-1;
  if(!(btn&BUTTON_UP)&&(lastbtn&BUTTON_UP)){
    rtn=UP;
  }
  if(!(btn&BUTTON_DOWN)&&(lastbtn&BUTTON_DOWN)){
    rtn=DOWN;
  }
  if(!(btn&BUTTON_RIGHT)&&(lastbtn&BUTTON_RIGHT)){
    rtn=RIGHT;
  }
  if(!(btn&BUTTON_LEFT)&&(lastbtn&BUTTON_LEFT)){
    rtn=LEFT;
  }
  lastbtn=btn;

  return rtn;
}

//finds the rank of a given score from the top 5 high scores if the score is not among top 5 then it returns -1
int8_t findRank(uint16_t score){
  for(int8_t i=0;i<5;i++){
    //uint16_t highScore=EEPROM.read(i*4+EEPROM_SCORE);
    uint16_t highScore;
    EEPROM.get(i*5+EEPROM_SCORE,highScore);
    if(score>highScore){
      return (i+1);
    }
  }

  return -1;
}

//generates a random sequence of N charachters from a set of M and prints it on the first line of lcd.
void generateRandomSequence(){
  for(int8_t i=0;i<N&&i<MAX_N;i++){
    S[i]=random(RIGHT,M);
    lcd.setCursor(0,1);

    switch(S[i]){
      case RIGHT: createRight(); break;
      case LEFT: createLeft(); break;
      case UP: createUp(); break;
      case DOWN: createDown(); break;
    }
    
    lcd.setCursor(0,0); lcd.write(4);
    
    lcd.setCursor(1,0); lcd.write(5);
    
    lcd.setCursor(0,1); lcd.write(6);

    lcd.setCursor(1,1); lcd.write(7);
    
    delay(currentD);

    lcd.clear();
    delay(150);
    
  }

  lcd.createChar(UP_DOWN,upDownArrow);
}

void createRight(){
  byte right0x0[] = {B10000,B11000,B11100,B11110,B11111,B11111,B11111,B11111};

  byte right0x1[] = {B00000,B00000,B00000,B00000,B00000,B10000,B11000,B11100};

  byte right1x0[] = {B11111,B11111,B11111,B11111,B11110,B11100,B11000,B10000};
  
  byte right1x1[] = {B11100,B11000,B10000,B00000,B00000,B00000,B00000,B00000};

  lcd.createChar(4,right0x0);
  lcd.createChar(5,right0x1);
  lcd.createChar(6,right1x0);
  lcd.createChar(7,right1x1);
  
  
}

void createLeft(){

  byte left0x0[] = {B00000,B00000,B00000,B00000,B00000,B00001,B00011,B00111};

  byte left0x1[] = {B00001,B00011,B00111,B01111,B11111,B11111,B11111,B11111};
  
  byte left1x0[] = {B00111,B00011,B00001,B00000,B00000,B00000,B00000,B00000};

  byte left1x1[] = {B11111,B11111,B11111,B11111,B01111,B00111,B00011,B00001};

  lcd.createChar(4,left0x0);
  lcd.createChar(5,left0x1);
  lcd.createChar(6,left1x0);
  lcd.createChar(7,left1x1);
  
}

void createUp(){
  byte up0x0[] = {B00000,B00001,B00011,B00111,B01111,B11111,B00001,B00001};

  byte up0x1[] = {B00000,B10000,B11000,B11100,B11110,B11111,B10000,B10000};

  byte up1x0[] = {B00001,B00001,B00001,B00001,B00001,B00001,B00001,B00000};

  byte up1x1[] = {B10000,B10000,B10000,B10000,B10000,B10000,B10000,B00000};
  
  lcd.createChar(4,up0x0);
  lcd.createChar(5,up0x1);
  lcd.createChar(6,up1x0);
  lcd.createChar(7,up1x1);
}

void createDown(){
  byte down0x0[] = {B00000,B00001,B00001,B00001,B00001,B00001,B00001,B00001};

  byte down0x1[] = {B00000,B10000,B10000,B10000,B10000,B10000,B10000,B10000};

  byte down1x0[] = {B00001,B00001,B11111,B01111,B00111,B00011,B00001,B00000};

  byte down1x1[] = {B10000,B10000,B11111,B11110,B11100,B11000,B10000,B00000};

  lcd.createChar(4,down0x0);
  lcd.createChar(5,down0x1);
  lcd.createChar(6,down1x0);
  lcd.createChar(7,down1x1);
}

void createReward(uint8_t n){
  byte c1[] = {B00000,B00100,B01110,B11111,B01110,B00100,B00010,B00001};

  byte c2[] = {B10001,B11111,B01110,B01010,B11111,B11111,B11111,B10001};
  
  byte c3[] = {B11011,B01010,B11111,B10101,B11111,B10001,B01110,B11111};
  
  byte c4[] = {B00000,B11111,B10101,B11111,B11011,B01110,B10001,B01110};
  
  byte c5[] = {B11111,B11111,B10001,B01010,B00100,B01110,B01010,B00000};
  
  byte c6[] = {B00101,B00111,B01111,B00111,B10111,B01111,B00101,B00101};
  
  byte c7[] = {B01111,B01111,B00101,B01110,B11111,B01110,B01010,B11011};
  
  byte c8[] = {B00000,B01110,B11111,B10101,B11111,B11111,B10101,B00000};
  
  byte c9[] = {B01010,B10101,B01010,B10101,B01010,B10101,B01010,B10101};
  
  byte c10[] = {B00111,B01110,B11100,B11000,B11100,B01110,B00111,B00000};
  
  byte c11[] = {B00111,B01111,B11111,B11111,B11111,B01111,B00111,B00000};
  
  byte c12[] = {B11111,B00100,B00100,B11111,B11111,B10001,B10001,B11111};
  
  byte c13[] = {B00000,B01100,B10100,B01110,B11111,B11111,B11111,B01110};
  
  byte c14[] = {B10000,B01000,B00111,B00111,B00111,B00010,B00010,B00111};
  
  byte c15[] = {B00000,B10001,B11111,B10001,B11111,B01110,B11111,B10001};
  
  byte c16[] = {B01110,B10001,B10001,B11111,B10101,B10101,B10001,B11111};
  
  byte *chars[TOTAL_REWARDS]={c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15,c16};

  int i=n%TOTAL_REWARDS;
  lcd.createChar(7,chars[i]);
}
