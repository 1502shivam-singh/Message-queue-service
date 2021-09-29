# Message-queue-service
A bare-bones implementation of a message queue service through Winsock2 API.

# Contribute

Steps to contribute

- Fork this repository and clone down your copy in your system. 

- Setting the proper upstream. Switch to your forked master branch, pull codes from upstream, then create a new branch.

      $ git remote add upstream https://github.com/1502shivam-singh/Message-queue-service.git
      $ git checkout master
      $ git pull upstream master
      $ git checkout -b IssueNo


- As the service is written using the Winsock2 library, Windows would be a better OS for running this.
For BSD OSs compatibility, some basic porting will be required (Mostly for WSA... type functions)

- After coding, then push codes to your fork repo.

      $ git add modified-file-names
      $ git commit -m 'commit message'
      $ git push origin issueNo

- Finally submit a PR

# System design

Message queue system																															
																																				
### Design overview
The service.cpp is a multithreaded server node 
																																				
---- t1 thread : dequeueing recevied messages and sending to receiver after some wait (randomised, generated to mimic actual wait)			
																																				
---- t2 thread : receiving message from client and enqueuing in the message queue													            
																																				
### System working																																
Client keeps sending data to this service on server																										
Server receives the data and queues it in a queue on one thread and other thread routes these messages after dequeuing	
