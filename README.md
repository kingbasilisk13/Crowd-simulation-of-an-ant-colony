# Crowd simulation of an ant colony.

## Abstract:
Machine learning and AI enable us to program decision-making for non-player characters (NPCs) in video games. They can also be used to simulate the behavior of real-world animals. In the following research project, I will attempt to simulate the workings of an ant colony using crowd simulation. I will use a combination of steering behaviors, decision-making, and influence maps to create the simulation model.

## Introduction:
Did you know that ants can be found nearly everywhere on earth, except Antarctica and a few inhospitable islands. This fact clearly indicates that ants are adapt at spreading to new places and surviving there. While this phenomenon poses challenges to the local ecosystems disrupted by the ants, it falls outside the scope of this project.
The point is that ants display remarkable teamwork. Their nests consist of multiple chambers, each serving their own purpose: A chamber where the queen resides, brood chambers ordered by the age of the brood, and garbage sites for disposing of leftover food and dead ants. These garbage sites are created to prevent the smell from attracting predators and to prevent the growth of mold within the nest. 
Ants also have the ability to store food. To achieve this, ants make use of their social stomachs to store food. They then use a process called trophallaxis to regurgitate food from one ant to another. They accomplish all of this and more while being nearly blind. Instead of relying on sight and sound to convey meaning to one another, they use specialized pheromones. For example: to indicate where an ant found food, they lay down a pheromone trail. They also use this trail to find their way back home. When an ant dies, they start releasing a pheromone that alerts nearby ants of the dead ant. The colony then collects the body and brings it to the garbage site. There are around 10 to 20 pheromones that ants use, each with its own meaning.
This is only a general description, and there are hundreds of ant species, each with its own unique quirks. For example, there are leaf cutter ants that make their nests in trees and Dracula ants that survive by drinking a small amount of blood from their own brood. For this project, I will limit myself to the following behaviors.

1. Queen: The queen ant is a stationary ant that produces new ants every X seconds. She cannot get food herself and thus will need to be fed by worker ants.
2. Worker ants: These ants are the most common and do most of the work. They go out and forage for food, collecting it in their social stomach. Upon returning to the colony, they feed the queen and other ants. Worker ants have a limited lifespan and will die. They move dead ants to the garbage site.
3. Soldier ants: Larger than worker ants, these ants are tasked with protecting the colony. They react to external threats and intruders, defending the colony.

To achieve these behaviors, the following components will be used to create a crowd simulation model:
* Steering behaviors: This provides a way to control how the ants move.
* Decision making: This allows the ants to decide what actions to take based on their current situation.
* Influence map: This serves as a substitute for pheromones, enabling the ants to map out their surroundings without the need for pathfinding.

The project will make use of a framework provided by the teachers of gameplay programming at DAE. This framework already contains working code for the previous components, which I will use as a base for the project.

## Crowd Simulation
Crowd simulation is the process of simulation the movement and/or dynamics of a large number of entities. It is most commonly used to create realistic scenes of large crowds in movies and video games. It can also be used in crisis training, architecture and urban planning, and evacuation simulation. 

In all technicality crowd simulation focuses primarily on simulating human-like entities. So saying that this project uses crowd simulation is incorrect. But the decision making of the ants is more detailed that it could nearly be considerds crowd simulation.

There are several approaches to crowd simulation, each with there own advantages and disadvantages based on the following two factors: crowd size and time scale.
* Crowd size: this refers to the number of entities in the simulation. There is no exact number to specify if a crowd is large or small.
* Time scale: this refers to how the objective of the simulation effects the length of the simulation. For example the simulation of how ideologies spread amongst a population will run for a long time as these events can take months or even years.

These 2 factors are used to classify the folowing crowd simulations.
* Flow-based Approach: a simulation focused on a crowd as a whole. Individuals have no distinct behavior that occurs due to input from their surroundings. Mostly used to simulate the flow of movement. Crowd size: large, Time Scale: short.
* Entity-based Approach: a simulation that implements a set of physical, predefined, and global laws. The entities do not think for themselves and their movement and actions are determined by the global laws. This model is most often used to research crowd dynamics such as jamming and flocking. Crowd Size: Small to medium, Time Scale: short.
* Agent-based Approach: Each agent in this simulation has a degree of intelligence. They can each react to each situation on their own based on a set of decision rules. They also use information from there surrounding to decide how to react. This is most often used to simulate realistic crowds. Crowd size and time scale depend entirely on what the researcher is trying to simulate.

This project falls somwhere between entity-based and agent-based approach. All ants will folow a predefined global law but they can also make decisions individualy based upon there suroundings.

## Components in more detail
### Steering behaviors:
Steering behaviors are an amalgamation of different behaviors used to manage the movement of an AI. These are often very simple behaviors that utilize external factors such as goals, other agents, and obstacles to move the agent. Here are a few examples:

* Seek: Move to a specific location.
* Flee: Move away from a specific location.
* Pursuit: Predict the location where another moving agent will be in the near future and move to that location.
* Evade: The opposite of pursuit. Move away from the predicted location of another moving agent.
* Wander: Choose a random location in front of the agent and move towards it.

On their own, these behaviors are very limited, but combining them opens up a whole new world of possibilities. We will limit ourselves to the following two ways of combining steering behaviors: priority steering and blended steering.

#### Priority steering:
Priority steering uses additional rules and checks to determine which steering behavior to prioritize. For example, if you want an agent that wanders around randomly but also evades obstacles, you can create a priority steering behavior using wander and evade. By default, the agent will use the wander behavior to move around, but the moment the agent detects an obstacle, it will switch to the evade behavior to move away from it.

#### Blended steering:
Blended steering assigns a certain weight to each steering behavior. The higher the weight of the behavior, the more influence it will have on the movement of the agent. For example, if you combine the wander behavior and the seek behavior, each with a weight of 50%, you will get "drunk seek": an agent that tries to reach a specific point while never following a straight path.

### Decision making:
An agent should have the ability to choose what to do in certain situations. This is mostly manually programmed, but doing this for every situation is hard and time-consuming. It also results in code that contains giant if-else statements. To avoid this, several ways of decision-making have been developed to simplify and clean up this process. We will limit ourselves to Finite State Machine and Behavior Trees.

#### Finite State Machine:
A finite state machine is a directed graph with each node representing a state in which the agent can be, and every link representing a transition to another state. An agent can only be in one state at a time and can only transition to another state to which it has a link. An example is the finite state machine used for deciding what animation to play at what time. This is, of course, very simplified but suffices for the current project.

#### Behavior tree:
A behavior tree is a directed acyclic graph, a directed graph with no cycles. The tree always starts at the same point, the root, and then makes its way down the tree. Depending on the results of the nodes, the agent will perform certain actions, or the program will return back down the tree. The nodes that return a result are called action nodes and can return one of the following three actions: succeeded, failed, or running. This result is sent to the composite node. This node decides what to do depending on the result of the action node. We will limit ourselves to the following two composite nodes: the sequence node and the selector node.

##### The sequence node:
The sequence node attempts to execute each action node in the given order. Depending on the results of the action nodes, the sequence executes a specific action:
* Action node returns success: The sequence node starts executing the next action node.
* Action node returns running: The sequence node aborts the execution and returns running.
* Action node returns failed: The sequence node aborts the execution and returns failed.
You can compare this node to an AND operation.

##### The selector node:
The selector node attempts to execute each action node until a certain result is received:
* Action node returns success: The selector node stops and returns success.
* Action node returns running: The selector node stops and returns running.
* Action node returns failed: The selector node starts executing the next child.
Just like the sequence node, you can compare this node to the OR operation.

Action nodes are used to make the agent do something and cannot have child nodes. This can be very limiting, especially if in a sequence you want to check if a certain condition is met before executing the rest of the sequence. To solve this, we use Conditional nodes. Conditional nodes are nodes used to check a condition, and they return either true or false.

Combining this with the movement behavior from before will handle most of the decision-making and movement.

### Influence maps
Influence maps are a collection of the influence that agents have on the world. It is a spatial storage of influence that agents can use to make decisions. For example, if a lot of enemy agents have walked through the same area, their influence on that area will be significant, and an agent may know not to go there. This helps a group of agents make decisions more strategically. Different information needs to be stored in separate influence maps. Influence maps can also be used to store information from the past, predict agents' future positions, and give agents imperfect information. Influence spreads over the map; we call this influence propagation. There are multiple ways to propagate, but we will limit ourselves to the following two: convolution filters and map flooding.

#### Convolution filters:
A convolution filter is a blurring technique. Using a small matrix of numbers, also known as a kernel, on each pixel of an image will result in a new image where each pixel is a weighted sum of the pixels in the original image. Using this on an influence map means that each tile on the map will hold the weighted influence of the tiles in the previous iteration.
The advantage of this is that each tile gets processed only once per iteration.
The disadvantage of this is that it cannot combine influences into a new higher influence. It also does not work well when there are only a small number of influence sources.

#### Map flooding:
Map flooding uses the flood fill algorithm to spread influence across the map. The flood fill algorithm is an algorithm that determines and alters the area connected to a given node. An example of this is the bucket tool in MS Paint.
The advantage of this is that it is fast and can use terrain analysis to avoid obstacles.
The disadvantage of this is that it is not good at combining influences because it picks the highest one.
Combining these two will result in influence propagation that will take the highest influence value of all neighbors and interpolate with the current influence. To control this, two parameters are used: Decay and Momentum.
* Momentum: Indicates how difficult it is to modify the value of something. For example, a car with high momentum will not stop easily. Higher momentum will result in slower propagation.
* Decay: How much influence decreases over distance. Lower decay will result in further spread of the influence.


## Creating the project
To create the crowd simulation the folowing components are used:
* Blended steering for movement.
* Influence map for communication between ants.
* Behavior tree for decision making.

Before actually writing the program, here is a short description of how it is going to work. The main class will create a starting number of ants: 1 queen, 20 soldiers, and 50 worker ants. All the ants will be stored in a vector that stores objects of the type AntBase. After that, several influence maps will be created: food map, home map, garbage map, hunger map, and threat map. This information will be added to a blackboard that will contain all the necessary information for the behavior tree: the vector with all the ants, a pointer to the ant that is currently being handled. Using this blackboard, the behavior tree will be created. The tree will have a branch that contains the decisions for the type of ant that is being handled. Finally, during the update function, the program will go through the vector of ants, setting each ant as the handled ant in the blackboard and executing the behavior tree before going to the next ant in the vector. 



### Creating the ants
To create the different ants a base class will be used where the other ants will be derived from.



