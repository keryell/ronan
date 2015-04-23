library IEEE;
use IEEE.std_logic_1164.all;

--------------------------------------------------------------------
--	cellule filtre du premier ordre pour le filtre de Deriche --
--------------------------------------------------------------------

entity filtre is
    generic (lambda : STD_LOGIC_VECTOR (2 downto 0) :="010"; n : INTEGER :=22);
    port (
    	reset: in STD_LOGIC;
        clk: in STD_LOGIC;
        x: in STD_LOGIC_VECTOR (n-1 downto 0);
        y: out STD_LOGIC_VECTOR (n+1 downto 0);
        Load: in STD_LOGIC
        );
end filtre;

architecture filtre_arch of filtre is

	signal R1,R2,R2b,R3 : STD_LOGIC_VECTOR ((n-1) downto 0);
	signal R4,R5,R5b : STD_LOGIC_VECTOR ((n) downto 0);
	signal R6,R7 : STD_LOGIC_VECTOR ((n+1) downto 0);

	component Adder 
    		generic (n : INTEGER :=8);
    		port (
        		a: in STD_LOGIC_VECTOR (n-1 downto 0);
        		b: in STD_LOGIC_VECTOR (n-1 downto 0);
        		S: out STD_LOGIC_VECTOR (n downto 0)
         	);
        end component;

begin
    
	-- Multiplexeurs --
	
	R1 <= x when lambda(0)='0' else R7((n+1) downto 2);
	R2 <= x when lambda(1)='0' else R7((n+1) downto 2);
	R3 <= x when lambda(2)='0' else R7((n+1) downto 2);

	-- Additionneurs --
	
    add1:
    	Adder 	generic map (n)
    		port map (R3, R2b, R4);
    		
    add2:
    	Adder 	generic map (n)
    		port map (R1, x, R5);
    		
    add3:
    	Adder 	generic map (n+1)
    		port map (R4, R5b, R6);
    	
  	-- Shifters --
  
  	R2b<=('0' & R2((n-1) downto 1));
  	R5b<=("00" & R5(n downto 2));
  
	-- Registre --
	
	process (clk, reset)
		begin
			if reset='1' then R7<=(others=>'0');
				elsif (clk'event and clk='1') then
					if (Load='1') then
						R7<=R6;
						end if;
			end if;
		end process;
  
  	-- Sortie --
  	
  	y<=R7;
  
  
end filtre_arch;








