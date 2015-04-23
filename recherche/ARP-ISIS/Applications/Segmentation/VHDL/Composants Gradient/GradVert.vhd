--------------------------------------------------------------------
--   Extracteur de gradient vertical : (1+Z^(-1))		  --
--------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
library METAMOR;
use METAMOR.attributes.all;
use METAMOR.array_arith.all;
library SYNOPSYS;
use SYNOPSYS.std_logic_arith.all;
use SYNOPSYS.std_logic_unsigned.all;

entity GradVert is
    generic (n : INTEGER :=18);
    port (
        DATAIN: in STD_LOGIC_VECTOR (n-1 downto 0);
        DATAOUT: out STD_LOGIC_VECTOR (n downto 0);
        LOAD : in STD_LOGIC;
        CLK : in STD_LOGIC
    );
end GradVert;


architecture GradVert_archX of GradVert is

signal retard : STD_LOGIC_VECTOR (n-1 downto 0);

begin

process(clk)
begin
	if rising_edge(clk) and load='1' then
		retard<=DATAIN;
		end if;
end process;

	DATAOUT <= (DATAIN(N-1) & DATAIN) + (retard(N-1) & retard);

end GradVert_archX;
