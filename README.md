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


**Decision**

The first design consideration makes more sense because it helps handle the namespaced processes better. Take the following case as an example: 

1. The container process is launched in the new namespaces as the init process. 
2. The container process launches a child process. 
3. The child process does its work and creates more children. 
4. The child process dies unexpectedly.
5. The orphaned childen can either:
    - Become the children of the container's init process. 
    - Become the children of the host's init process. 
6. Handling it in the first case makes more sense and clearly defines the scope of the container.
7. The second design consideration is more like a wrapper around the container process.