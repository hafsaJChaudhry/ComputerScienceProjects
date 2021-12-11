% optm.m  find minimum
function optm
  format compact
  zz=0;
  for xx=-1.0:0.01:1.0    
    for yy=-1.0:0.01:1.0     
      vv=[ xx yy ];  
      a=fminsearch(@f,vv);  
      best=f(a); 
      if(best<zz)    
        aa=a;
        zz=best;
      end
    end
  end
 
  zz
  zz=0;
  for xx=-1.0:0.01:1.0
    for yy=-1.0:0.01:1.0
      vv=[ xx yy ];      
      a=fminsearch(@f,vv,optimset('MaxFunEvals',1000000));      
      best=f(a);      
      if(best<zz)
        aa=a;
        zz=best;
      end     
    end    
  end

  vpa(aa, 100)
  vpa(zz, 100)
  
  return

  function z=f(v)
      
    x=v(1);
    
    y=v(2);
    
    z = exp(sin(50.0*x)) + sin(60.0*exp(y)) + sin(80.0*sin(x)) + sin(sin(70.0*y)) - sin(10.0*(x+y)) + (x*x+y*y)/4.0
    
    return
    
  end % f

end % optm

% x = 0.46865720042324399141620006048469804227352142333984375
% y = -0.9229261042163849904085282105370424687862396240234375
% z = -3.138333097015244987915139063261449337005615234375
