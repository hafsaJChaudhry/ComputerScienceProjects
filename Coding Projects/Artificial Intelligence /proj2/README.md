% FILE: estimate.m implemenets a consensus-based estimation protocol
%
% DESCRIPTION:
% Each agent updates their own estimate of a sensor value by applying the
% consensus protocol with its neighbors.
%
% INPUTS:
% se_in - everyone's original sensor estimates
%
% OUTPUTS:
% delta_est - the change in estimate required to arrive at a consensus with
% neighbors about the estimate
%
% TODO:
% Handle estimates from unknown, bad faith actors



% FILE: swarm.m implements a Boids-like behavior
%
% DESCRIPTION:
% Boids-like repulsion-orientation-attraction behavior based on the 
% behavior described by Couzin et al. in the Collective Memory paper. 
%
% INPUTS:
% rr - radius of repulsion
% ro - radius of orientation
% ra - radius of attraction
% x - matrix containing the pose of all the robots
% neighbors (NOT USED) - matrix containing neighborhood information for each robot
% blind_neighbors - matrix tracking the robots in a robot's blind spot
% N - the number of robots in the swarm
% dxi - the original velocity of all the robots
%
% OUTPUTS:
% vel - the resulting velocity of the robots
%
% TODO:
% 1. orient matrix
% 2. attract matrix
% 3. dxi(1:2, i) for orientation
% 4. dxi(1:2, i) for attraction






# MATLAB Version 

Currently, we know that the simulator works with MATLAB 2014b and higher.  The backwards compatibility issues are mostly due to changes in the way MATLAB handles figures in newer releases.

# Required Toolboxes 

We make heavy use of MATLAB's optimization toolbox function 'quadprog.'  Though this toolbox isn't necessarily required, all of our barrier function algorithms utilize this function.

# robotarium-matlab-simulator
MATLAB simulator for the Robotarium!  The purpose of the Robotarium simulator is to ensure that algorithms perform reasonably well before deployment onto the Robotarium.  Note that scripts created for the simulator can be directly deployed onto the Robotarium.  To ensure minimum modification after deployment, the simulator has been created to closely approximate the actual behavior of the Robotarium's agents. 

# Usage 

First, take a look at the "examples" folder for a few, simple examples.  Note that, to run these examples, you must first run the "init.m" script to add the requisite directories.  

# Documentation 

For example mathematical documentation, FAQs, and more, visit http://www.robotarium.org.
