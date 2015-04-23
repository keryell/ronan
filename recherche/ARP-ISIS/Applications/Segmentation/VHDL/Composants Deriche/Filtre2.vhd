library IEEE;
use IEEE.std_logic_1164.all;

--------------------------------------------------------------------
--	cellule filtre du premier ordre pour le filtre de Deriche --
--------------------------------------------------------------------

entity filtre2 is
    generic (lambda : STD_LOGIC_VECTOR (2 downto 0) :="010"; n: INTEGER :=16; m: INTEGER :=17);
    port (
    	reset: in STD_LOGIC;
        clk: in STD_LOGIC;
        x: in STD_LOGIC_VECTOR(n-1 downto 0);
        y: out STD_LOGIC_VECTOR(m-1 downto 0);
        Load: in STD_LOGIC
        );
end filtre2;

architecture filtre2_arch of filtre2 is

	signal R0,R1,R2,R3 : STD_LOGIC_VECTOR((m-1) downto 0);
	signal R5, R2b, R3b : STD_LOGIC_VECTOR(m downto 0);
	signal R4 : STD_LOGIC_VECTOR((m+1) downto 0);
	signal R4b, R5b : STD_LOGIC_VECTOR((m+2) downto 0);
	signal R6 : STD_LOGIC_VECTOR((m+3) downto 0);
	signal R7 : STD_LOGIC_VECTOR((m-1) downto 0);
	
	component Adder 
    		generic (n : INTEGER :=8);
    		port (
        		a: in STD_LOGIC_VECTOR (n-1 downto 0);
        		b: in STD_LOGIC_VECTOR (n-1 downto 0);
        		S: out STD_LOGIC_VECTOR (n downto 0)
         	);
        end component;

begin

	R0((m-1) downto (m-n)) <= x(n-1 downto 0);
	R0(((m-n)-1) downto 0) <= (others => '0');
    
	-- Multiplexeurs --
	
	R1 <= R0 when lambda(0)='0' else R7;
	R2 <= R0 when lambda(1)='0' else R7;
	R3 <= R0 when lambda(2)='0' else R7;

	-- Shifters --
	
	R2b <= ("0" & R2);
	R3b <= (R3 & "0");
	R4b <= (R4 & "0");
	R5b <= ("00" & R5);

	-- Additionneurs --
	
    add1:
    	Adder 	generic map (m+1)
    		port map (R3b, R2b, R4);
    		
    add2:
    	Adder 	generic map (m)
    		port map (R1, R0, R5);
    		
    add3:
    	Adder 	generic map (m+3)
    		port map (R4b, R5b, R6);
    	
	-- Registre --
	
	process (clk, reset)
		begin
			if reset='1' then R7<=(others=>'0');
				elsif (clk'event and clk='1') then
					if (Load='1') then
						R7<=R6((m+2) downto 3);
						end if;
			end if;
		end process;
  
  	-- Sortie --
  	
  	y<=R7;
  
  
end filtre2_arch;








