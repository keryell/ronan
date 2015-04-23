library IEEE;
use IEEE.std_logic_1164.all;
library METAMOR;
use METAMOR.attributes.all;
use METAMOR.array_arith.all;
library SYNOPSYS;
use SYNOPSYS.std_logic_arith.all;
use SYNOPSYS.std_logic_unsigned.all;

--------------------------------------------------------------------
--   Additionneur n bits a retenue propagee sans carry entrante   --
--------------------------------------------------------------------

entity Adder is
    generic (n : INTEGER :=8);
    port (
        a: in STD_LOGIC_VECTOR (n-1 downto 0);
        b: in STD_LOGIC_VECTOR (n-1 downto 0);
        S: out STD_LOGIC_VECTOR (n downto 0)
    );
end Adder;

--------------------------------------------------------------------
--       version sans propagation rapide de la retenue            --
--------------------------------------------------------------------

--architecture Adder_arch of Adder is

--signal c : STD_LOGIC_VECTOR (n downto 0);

--begin
--	c(0)<='0';
--  eti1:
--  	for i in 0 to n-1 generate
--  		S(i)<=a(i) xor b(i) xor c(i);
--  		c(i+1)<=(a(i) and b(i) and not c(i)) or (  (a(i) or b(i)) and c(i)  );
--  		end generate;
--  	S(n)<=c(n);
--end Adder_arch;

--------------------------------------------------------------------
--       version avec propagation rapide de la retenue            --
--------------------------------------------------------------------

architecture Adder_archX of Adder is

begin
	S <= ("0" & a) + ("0" & b);
end Adder_archX;
