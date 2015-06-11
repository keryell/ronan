---------------------------------------------------------------
--   Memoire ligne : ecriture gauche -> droite               --
--                   lecture  droite ->gauche                --
---------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
library SYNOPSYS;
use SYNOPSYS.std_logic_arith.all;
use SYNOPSYS.std_logic_unsigned.all;

entity mem_lign is
    generic (N : integer := 8; M : integer := 16);
    port (
        data_in: in STD_LOGIC_VECTOR (N-1 downto 0);
        data_out: out STD_LOGIC_VECTOR (N-1 downto 0);
        GenClk1: in STD_LOGIC;
        load: in STD_LOGIC;
        reset: in STD_LOGIC
    );
end mem_lign;

architecture mem_lign_arch of mem_lign is

type dataram is array (M-1 downto 0) of std_logic_vector(N-1 downto 0);

signal A : std_logic_vector(8 downto 0):=(others=>'0');
signal D : std_logic_vector(N-1 downto 0);
signal Q_out : std_logic_vector(N-1 downto 0);
signal WE : std_logic_vector(M-1 downto 0);
signal Q : dataram;
signal AL : std_logic_vector(4 downto 0);
signal AH : std_logic_vector(3 downto 0);
signal pair : std_logic:='0';
signal clk : std_logic;

component RAM32X1S
   port (A0, A1, A2, A3, A4, D : in std_logic;
   	O : out std_logic;
   	WCLK, WE : in std_logic);
end component ;

   component BUFG
     port (I : in std_logic; O : out std_logic);
   end component;

begin

   H1: BUFG port map (I=>GenClk1, O=>Clk);
  
-- Generation de l'espace memoire

RAMM: 	for j in M-1 downto 0 generate
	WE(j) <= LOAD when (unsigned(AH)=j) else '0';
	RAMN: 	for i in N-1 downto 0 generate
      		RAM1: RAM32X1S
   			port map (A0 =>AL(0), A1=>AL(1), A2=>AL(2), A3=>AL(3), A4=>AL(4),
   				 D=>D(i), O=>Q(j)(i), Wclk=>Clk ,WE=>WE(j));
   		end generate;
 	end generate;

Q_out <= Q(conv_integer(AH))(N-1 downto 0);
 	
AH <= A(8 downto 5);
AL <= A(4 downto 0);

-- definition des registres d'entree/sortie et adressage de la memoire

process(clk)
	begin
	if rising_edge(Clk) then
		if (Load='1') then
		D <= Data_in;
		Data_out <= Q_out;
		if (pair='0') then
				if (A="111111111") then
					pair <= '1';
					else
					A<=A+1;
				end if;
				else
				if (A="000000000") then
					pair <= '0';
					else
					A<=A-1;
				end if;
			end if;
		end if;	
	end if	;
	end process;

end mem_lign_arch;