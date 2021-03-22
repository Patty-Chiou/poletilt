# poletilt
IoT CC1310

#### GOAL
report acceleration status with preferred time interval or order from server   and alarm server when triggered

#### HOW
interaction between gateway and node
##### gateway 
- ask node one by one  with a preferred time interval
[using switch cases and flag]
- sent packages from nodes back to server through NB module

##### node 
- listen to gateway's order ON/OFF power to trigger accelerometer and 
- sent packages received from accelerometer to gateway


#### SKILLS
- Clear and well design functions
- CC1310 TxRx 

#### SOLVING
- If the accelerometer is triggered, we can know which node is in emergency and rescue to the right place right away.
- To know the status of each node without engineers' regular commute to the rural area to check if it is ok.
