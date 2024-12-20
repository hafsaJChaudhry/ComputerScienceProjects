function delta_est = estimate(se_in, neighbors, N)
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

%% Authors: Mohammad Safwan Alam, Musad Haque - 2019
%%%%%%%%%%%%%

delta_est = zeros(N, 1);

for ii = 1:1:N
    % looks at all agents
    for jj = 1:1:N
        % checks if other agent isn't you, and is your neighbor
        if ((ii ~= jj) && neighbors(ii, jj))
            %Regular consensus with neighbors
            %Susceptible to bad faith actors
            
            % for each neighbor, sums up the difference between your
            % reading and the other agent's reading
            diff_bw_ii_jj = se_in(ii, 1) - se_in(jj, 1);
            if (abs(diff_bw_ii_jj) < 1) 
                % if ii and jj are really close, run normal consensus
                delta_est(ii, 1) = delta_est(ii, 1) - diff_bw_ii_jj;
            else
                % if the difference betwen agents ii and jj is large, don't
                % let jj influence ii by much
                % aka loners dont have much influence 
                delta_est(ii, 1) = delta_est(ii, 1) - 1/diff_bw_ii_jj; %/exp(abs(diff_bw_ii_jj));
            end
            

            %!!!!!MODIFY HERE!!!!! 
            %to handle possible bad estimates from malicious actors!!!!!
            %Note. ii can only use local neighborhood information
            %so we need to figure out how to be consensing w/ everyone 
            % kinda.  the current code already runs consensus, we just have
            % to change it so it kinda ignores bad readings
            % one way to do this is to scale the differences between your
            % reading and your neighbor's reading
       end
   end
    
end
               
    


