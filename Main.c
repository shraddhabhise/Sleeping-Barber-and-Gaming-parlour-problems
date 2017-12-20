code Main

  -- OS Class: Project 3
  --
  -- <	Shraddha Bhise >
  --

-----------------------------  Main  ---------------------------------

  function main ()
	InitializeScheduler()
	--SleepingBarber()									--Call for sleeping Barber     
	GamingParlour()									--Call for Gaming Parlour
	ThreadFinish()
    endFunction
---------------------------SleepingBarber----------------------------
const
	CHAIRS= 5										--Declares a constant for Number of chairs										

enum E,S,B,F,L,start,end,X,o									-- Declares enum for customers status( E:entry,S:sit,B:Begin,F:finish,L:Leave) and for barber (start, ends)
var 
	CustomersThread: array [12] of Thread = new array of Thread { 12 of new Thread }	-- Declares array of Customers Thread for 12 customers
	BarberThread: Thread									-- Declares barber thread
	customers: int=0									
	barbers: int=0
	mutex: int=1
	waiting:int=0 
	CustomerSemaphore: Semaphore=new Semaphore						--Declares semaphore for barber
	BarberSemaphore: Semaphore=new Semaphore						-- Declares semaphore for customer
	MutexSemaphore: Semaphore=new Semaphore  						-- Declares binary semaphore which acts as a lock 
	status: array[13] of int								-- Declares a status array to store status of customers and barber
	chairsArray:array[5] of int								-- stores chairs status				
	index: int=0

  function wait(time: int)									--To add delay between the arrival of customers at same time in barber shop
    var i: int
    for i =  1 to time
    endFor
  endFunction

  function SleepingBarber()
    
	print("       Barber     1  2  3  4  5  6  7  8  9  10  11  12\n")

	CustomerSemaphore.Init(customers)							--Initializes semaphore responsible for barber
	BarberSemaphore.Init(barbers)								--Initializes semaphore responsible for customers
	MutexSemaphore.Init(mutex)								-- Initializes binary semaphore
	BarberThread=new Thread
	status= new array of int {13 of '?'}
	chairsArray=new array of int {5 of o }   

	--Initializes and make ready barber thread and customer threads 

	BarberThread.Init("Barber")								
        BarberThread.Fork(Barber,0)		

	CustomersThread[0].Init ("C1")
        CustomersThread[0].Fork (Customer, 1)

	CustomersThread[1].Init ("C2")
        CustomersThread[1].Fork (Customer, 2)

	CustomersThread[2].Init ("C3")
        CustomersThread[2].Fork (Customer, 3)

	wait(500)
        
	CustomersThread[3].Init ("C4")
        CustomersThread[3].Fork (Customer, 4)	

	CustomersThread[4].Init ("C5")
        CustomersThread[4].Fork (Customer, 5)

	wait(1000)

        CustomersThread[5].Init ("C6")
        CustomersThread[5].Fork (Customer, 6)	

	wait(10000)
	
        CustomersThread[6].Init ("C7")
        CustomersThread[6].Fork (Customer, 7)

	wait(60000)

        CustomersThread[7].Init ("C8")
        CustomersThread[7].Fork (Customer, 8)

        CustomersThread[8].Init ("C9")
        CustomersThread[8].Fork (Customer, 9)

	wait(2000)

        CustomersThread[9].Init ("C10")
        CustomersThread[9].Fork (Customer, 10)

        CustomersThread[10].Init ("C11")	
	CustomersThread[10].Fork (Customer, 11)

	CustomersThread[11].Init ("C12")  
        CustomersThread[11].Fork (Customer, 12) 
	
	ThreadFinish()

  endFunction

 function Barber(p:int)
	
	while true
		CustomerSemaphore.Down()							--Down on Customer semaphore to make sleep barber if no there are no customers
		MutexSemaphore.Down()								--Acquires lock 
		chairsArray[index]=o								-- to empty one consumed chair when barber takes 1 customer to cut hair
		index=(index-1) % CHAIRS							
		waiting=waiting-1								-- decreases count of global var waiting  
		BarberSemaphore.Up()								-- Up on Barber Semaphore to wake the customer
		MutexSemaphore.Up()								-- Release lock
		CutHair(p)									-- calls cut hair method
	endWhile

 endFunction

 function Customer(p:int)
		
		MutexSemaphore.Down()								-- acquires lock
		status[p]=E									-- indicates that the customer has entered
		PrintAllStatus(p)								-- print status of current customer
		if( waiting < CHAIRS)								-- checks if waiting customers are less then chairs available
						
			chairsArray[index]=X							-- to indicate that the chair is occupied chair fo current customer
			index=(index+1) % CHAIRS
			waiting=waiting+1							-- increase the waiting customer count by one

			status[p]=S								-- indicates that customer is sitting on waiting chairs
                        PrintAllStatus(p)

			CustomerSemaphore.Up()							-- up on Customer Semaphore to wake up barber
			MutexSemaphore.Up()							-- releases lock
			BarberSemaphore.Down()							--Down on Barber Semaphore to make customer sleep
			GetHairCut(p)								--calls get haircut method
                        status[p]=L								--indicates that the customer is leaving after haircut
                        MutexSemaphore.Down()							--Acquires lock to print status
        		PrintAllStatus(p)
        		MutexSemaphore.Up()							-- releases the lock
		else
			
			status[p]=L								-- indicates that the customer needs to leave if he has no chair to sit
			PrintAllStatus(p)
			MutexSemaphore.Up()							-- releases the lock
		endIf
endFunction 

 function CutHair(p:int)
	var i:int
	
        status[p]=start										-- to indicate that barber has started haircut
	MutexSemaphore.Down()									-- to acquire lock for printing status
	PrintAllStatus(p)
	MutexSemaphore.Up()									-- to release lock 
	
	-- to yeild and give chance to customers 
	for i=i to 100
		currentThread.Yield() 		
	endFor

	status[p]=end										-- to indicate that barber has done the haircut
	MutexSemaphore.Down()									-- to acquire lock for printing status
	PrintAllStatus(p)
	MutexSemaphore.Up()									-- to release lock
	 
endFunction

 function GetHairCut(p:int)
	var i:int
	status[p]=B										-- to indicate that customer has begin to cut his/her hair
	MutexSemaphore.Down()									 -- to acquire lock for printing status
	PrintAllStatus(p)
	MutexSemaphore.Up()									-- to release lock

	status[p]=F										-- to indicate that customer has finished cutting his/ her hair
	MutexSemaphore.Down()									 -- to acquire lock for printing status
	PrintAllStatus(p)
	MutexSemaphore.Up()									 -- to release lock

	--to yield and give chance to barber
	for i=i to 100
                currentThread.Yield()
        endFor

endFunction

function PrintAllStatus(j: int)
var
	  i:int
          p: int
	
	--to print chairs status

	for i=0 to 4

	  switch chairsArray[i]
	    case X:
		print("X")
		break
	    case o:
		print("o")
		break
	 endSwitch
	endFor
	
	-- to print customer and barber status

        if j != 0
           print ("          ")
        endIf
	for p = 1 to j
            print ("   ")
        endFor  
          switch status [j]
            case E:
              print ("E  ")
              break
            case S:
              print ("S  ")
              break
            case B:
              print ("B  ")
              break
	    case F:
	      print ("F  ")
	      break
            case L:
	      print ("L  ")
	      break

	    case start:
	      print ("  Start   ")
	      break
	    case end:
	     print ("  End     ")
	      break
		default: print ("   ")
          endSwitch
        nl ()
      endFunction


--------------------------------------------------------------------------Gaming Parlour---------------------------------------------------------
var
	
	playerGroup:array[8] of Thread=new array of Thread { 8 of new Thread }             --creates array of thread for player group of 8
	mon: GamingParlourClass                                                            -- declare object of GamingParlourClass

function GamingParlour()						

	mon= new GamingParlourClass							 -- create object of GamingParlourClass
	mon.Init()									 -- initialize object of GamingParlourClass
	

	print("Initializing Schedular:")
	
	--Initializes threads array for playerGroup and makes them ready to run	

	playerGroup[0].Init ("A")							
        playerGroup[0].Fork (groupPlaying, 4)

        playerGroup[1].Init ("B")
        playerGroup[1].Fork (groupPlaying , 4)

        playerGroup[2].Init ("C")
        playerGroup[2].Fork (groupPlaying , 5)

        playerGroup[3].Init ("D")
        playerGroup[3].Fork (groupPlaying , 5)

        playerGroup[4].Init ("E")
        playerGroup[4].Fork (groupPlaying  , 2)

        playerGroup[5].Init ("F")
        playerGroup[5].Fork (groupPlaying , 2)

        playerGroup[6].Init ("G")
        playerGroup[6].Fork (groupPlaying , 1)

        playerGroup[7].Init ("H")
        playerGroup[7].Fork (groupPlaying , 1)

endFunction
 
function groupPlaying(p:int)
var i:int

	for i=1 to 5						-- to make each group play 5 times
		mon.RequestDice(p)				-- calls RequestDice method
		currentThread.Yield()
		mon.ReleaseDice(p)				-- calls ReleaseDice method
		currentThread.Yield()
	endFor

endFunction

class GamingParlourClass
	superclass Object
		fields
			numberDiceAvail:int   			-- global var to maintain count of available dice
			top: Condition			        -- declares a condition var for a group at front of waiting list 
			remainingGroup:Condition		-- declares a condition var for a groups except one at front
			monitorLock: Mutex			-- declares monitors mutex
			numWaitingGroups:int			-- declares numWaitingGroups to maintain count of waiting groups
		methods
			-- declares methods for initializing, Requesting and releasing dice

			Init()
			RequestDice(requestDice: int)		
			ReleaseDice(returnDice: int )
			PrintDeskStatus(str: String, count: int)	
			
endClass

behavior GamingParlourClass

	method Init()
		
		-- initializes all declared fields

		numberDiceAvail=8
		numWaitingGroups=0
		top=new Condition
		remainingGroup=new Condition
		monitorLock=new Mutex
		top.Init()
		remainingGroup.Init()
		monitorLock.Init()

	endMethod 

	method RequestDice(requestDice:int)
		monitorLock.Lock()					-- acquires monitors lock
		self.PrintDeskStatus(" Requests : ",requestDice)	-- prints number of requested dice
		numWaitingGroups=numWaitingGroups+1			-- increments number of waiting groups
		if numWaitingGroups > 1					-- checks if number there are any waiting groups  
			remainingGroup.Wait(&monitorLock)		-- makes sleep remaining groups except one at front of waiting queue 
		endIf
		while numberDiceAvail< requestDice			-- checks for availability of dice for fulfilling request
			top.Wait(&monitorLock)				-- makes sleep the front of waiting list group 
		endWhile
		numberDiceAvail=numberDiceAvail-requestDice		-- updates number of dice after fulfilling request for dice
		numWaitingGroups=numWaitingGroups-1			-- decrements a waiting group number
		remainingGroup.Signal(&monitorLock)			-- signals remaining group of waiting groups
		self.PrintDeskStatus(" Proceed : ",requestDice)		-- prints the 
		monitorLock.Unlock()					-- releases monitors mutex
	endMethod

	method ReleaseDice(returnDice:int)
		monitorLock.Lock()						-- acquires monitors lock
		numberDiceAvail=numberDiceAvail+returnDice			-- updates number of dice after returning dice
		self.PrintDeskStatus(" Release and adds back: ",returnDice)	-- prints the returned dice 
		top.Signal(&monitorLock)					-- signals group at the front of waiting list to prevent starvation and give chance to thread in FIFO manner
		monitorLock.Unlock()						-- releases monitors mutex

	endMethod
 	
	method PrintDeskStatus(str: String, count: int)
 	
 	-- This method prints the current thread's name and the arguments.
 	-- It also prints the current number of dice available.
 	nl()
 	print (currentThread.name)
 	print (" ")
 	print (str)
 	print (" ")
 	printInt (count)
 	nl ()
	 print ("------------------------------Number of dice now avail = ")
 	printInt (numberDiceAvail)
 	nl ()
 	endMethod
endBehavior




endCode
