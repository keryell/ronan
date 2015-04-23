--------------------------------------------------------------------
--	   Operateur valeur absolue				  --
--------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
library METAMOR;
use METAMOR.attributes.all;
use METAMOR.array_arith.all;
library SYNOPSYS;
use SYNOPSYS.std_logic_arith.all;
use SYNOPSYS.std_logic_unsigned.all;

entity Abso is
    generic (n : INTEGER :=19);
    port (
        DATAIN: in STD_LOGIC_VECTOR (n-1 downto 0);
        DATAOUT: out STD_LOGIC_VECTOR (n-1 downto 0)
    );
end Abso;


architecture Abso_archX of Abso is

signal zero : STD_LOGIC_VECTOR (n-1 downto 0):=(others=>'0');

begin
	
	zero <=(others=>'0');
	
	DATAOUT<= DATAIN when (DATAIN(n-1)='0') else (zero - DATAIN);

end Abso_archX;
