# procman
Trying to mimic the functionalities of docker. 


## Design Considerations 

Currently, I have two design considerations in mind, need to validate them before implementing: 

1. A chain of processes. 
    - The daemon process first performs a clone call with the new set of namespaces. 
    - This process will act as the init process for the new namespace.
    - The reasoning is to have a process which will contain the namespace attributes before the container job is stared. 
    - Next, another process is launched by the init process which performs the container logic. 
    - The init process monitors the state of the container process and cleans up the resources when the container process exits. 

2. A single process. 
    - The daemon process launches a single process which performs the container logic. 
    - The daemon process monitors the state of the container process and cleans up the resources when the container process exits. 