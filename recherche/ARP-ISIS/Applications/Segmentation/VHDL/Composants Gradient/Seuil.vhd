--------------------------------------------------------------------
--	   Double seuillage					  --
--------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
library METAMOR;
use METAMOR.attributes.all;
use METAMOR.array_arith.all;
library SYNOPSYS;
use SYNOPSYS.std_logic_arith.all;
use SYNOPSYS.std_logic_unsigned.all;

entity Seuil is
    generic (n : INTEGER := 19; SH : integer := 128; SL : integer := 32);
    port (
        DATAIN: in STD_LOGIC_VECTOR (n-1 downto 0);
        DATAOUT: out STD_LOGIC_VECTOR (1 downto 0)
    );
end Seuil;


architecture Seuil_archX of Seuil is

begin
	
	DATAOUT<= "00" when conv_integer(DATAIN) < SL else
		"01" when (conv_integer(DATAIN) > SH) else "11";

end Seuil_archX;
