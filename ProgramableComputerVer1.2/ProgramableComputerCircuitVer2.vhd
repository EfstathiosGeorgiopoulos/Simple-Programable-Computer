--Program Counter
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

entity cout is
	port (i : in std_logic_vector (7 downto 0);
			o : out std_logic_vector (7 downto 0);
			PCO, J, CE, clk : in std_logic);
end entity cout;

architecture coutt of cout is 

signal c : std_logic_vector (7 downto 0) := "00000000";

begin
process(PCO, clk)
	begin
	if PCO = '1' then
		if rising_edge(clk) then
			o <= c;
		end if;
	else
		o <= "UUUUUUUU";
	end if;
end process;
process(J, C, clk, i)
	begin
		if J = '1' then
			if clk = '1' then
				c <= i;
			end if;
		elsif CE = '1' then
			if rising_edge(clk) then
				c <= c + 1;
			end if;
		else
			c <= c;
		end if;
end process;
end architecture coutt;

--Register 
library ieee;
use ieee.std_logic_1164.all;

entity reg is
	port (i : in std_logic_vector (7 downto 0);
			o, otoU : out std_logic_vector (7 downto 0);
			load, en, clk : in std_logic);
end entity;

architecture arc of reg is 

signal data : std_logic_vector (7 downto 0) := "00000000";

begin
process(i, load, en, clk)
	begin
		if (load = '1' and rising_edge(clk)) then
			data <= i;
		else 
			data <= data;
		end if;
		if en = '1' then
			o <= data;
		else
			o <= "UUUUUUUU";
		end if;
		otoU <= data;
end process;
end architecture arc;

--Instruction register
library ieee;
use ieee.std_logic_1164.all;

entity inreg is
	port (i : in std_logic_vector(7 downto 0);
			II, IO, clk, enNew : in std_logic;
			po, co, instruction_current_data : out std_logic_vector(7 downto 0));
end entity inreg;

architecture arc of inreg is

signal data : std_logic_vector (7 downto 0) := "00000000";
signal store_instruction : std_logic_vector (7 downto 0);

begin

process(clk, IO, data)
	begin 
		if rising_edge(clk) then
			if IO = '1' then
				po <= "0000"&store_instruction(3)&store_instruction(2)&store_instruction(1)&store_instruction(0);
			else
				po <= "UUUUUUUU";
			end if;
			if enNew = '1' then
				store_instruction <= data;
			else 
				store_instruction <= store_instruction;
			end if;
			co <= store_instruction(7)&store_instruction(6)&store_instruction(5)&store_instruction(4)&"0000";
		end if;
		instruction_current_data <= store_instruction;
end process;

process(clk, i, II)
	begin
		if clk = '1' and II = '1' then
			data <= i;
		else
			data <= data;
		end if;
end process;

end architecture arc;

--Output register
library ieee;
use ieee.std_logic_1164.all;

entity reg is
	port (i : in std_logic_vector (7 downto 0);
			o, otoU : out std_logic_vector (7 downto 0);
			load, en, clk : in std_logic);
end entity;

architecture arc of reg is 

signal data : std_logic_vector (7 downto 0) := "00000000";

begin
process(i, load, en, clk)
	begin
		if (load = '1' and rising_edge(clk)) then
			data <= i;
		else 
			data <= data;
		end if;
		if en = '1' then
			o <= data;
		else
			o <= "UUUUUUUU";
		end if;
		otoU <= data;
end process;
end architecture arc;

--Flag register
library ieee;
use ieee.std_logic_1164.all;

entity flag is
	port (i : in std_logic_vector (1 downto 0);
			FI, clk : in std_logic;
			o : out std_logic_vector (1 downto 0));
end entity;

architecture arc of flag is
signal data : std_logic_vector (1 downto 0) := "00";
begin
process(i, FI, clk)
	begin
		if FI = '1' and rising_edge(clk) then 
			if i(0) = 'U' then 
				data(0) <= '0';
			else 
				data(0) <= i(0);
			end if;
			if i(1) = 'U' then 
				data(1) <= '0';
			else 
				data(1) <= i(1);
			end if;
		end if;
		o <= data;
end process;
end architecture arc;

--Zero flag statement
library ieee;
use ieee.std_logic_1164.all;

entity zerof is
	port (i : in std_logic_vector(7 downto 0);
			clk : in std_logic;
			o : out std_logic);
end entity zerof;

architecture arc of zerof is 
begin
o <= (i(0) nor i(1))and(i(2) nor i(3))and(i(4) nor i(5))and(i(6) nor i(7)) when rising_edge(clk);
end architecture arc;

--Bus register
library ieee;
use ieee.std_logic_1164.all;

entity busreg is
	port (ram, inreg, procount, regA, regB, ALUB : in std_logic_vector (7 downto 0);
			RO, PCO, AO, BO, EO, IO : in std_logic;
			o : out std_logic_vector (7 downto 0));
end busreg;

architecture busregg of busreg is
begin
process(RO, PCO, AO, BO, EO, IO, ram, inreg, procount, regA, regB, ALUB)
begin
	if PCO = '1' then
		o <= procount;
	elsif RO = '1' then
		o <= ram;
	elsif IO = '1' then
		o <= inreg;
	elsif AO = '1' then
		o <= regA;
	elsif BO = '1' then
		o <= regB;
	elsif EO = '1' then
		o <= ALUB;
	else
		o <= "UUUUUUUU";
	end if;
end process;
end architecture busregg;

--Clock control unit
library ieee;
use ieee.std_logic_1164.all;

entity clock is
	port (clk, HLT : in std_logic;
			o, orev : out std_logic);
end entity;

architecture arc of clock is

signal state : std_logic := '1';

begin

process(clk)
begin
	if HLT = '1' then
		state <= '0';
	end if;
	o <=  clk and state;
	orev <= (not clk) and state;
end process;

end architecture arc;

--Control Unit (CPU)
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_textio.all;
use std.textio.all;

entity CPU is
	port (instruction : in std_logic_vector(7 downto 0);
			clk : in std_logic;
			flags : in std_logic_vector (1 downto 0);
			enNewInstr : out std_logic;
			o : out std_logic_vector (15 downto 0));
end entity CPU;

architecture CPUU of CPU is

signal address : std_logic_vector(9 downto 0);
signal count : std_logic_vector(3 downto 0) := "0000";

-- ROM block 8x256
type RAM is array (integer range <>)of std_logic_vector (15 downto 0);
signal mem : RAM (0 to 1024);

--Fill ROM block
procedure Load_ROM (signal data_word :inout RAM) is
	file romfile   :text open read_mode is "memory.list";
	variable lbuf  :line;
	variable i     :integer := 0;
	variable fdata :std_logic_vector (15 downto 0);
begin
	while not endfile(romfile) loop
	readline(romfile, lbuf);
	read(lbuf, fdata);
	data_word(i) <= fdata;
	i := i+1;
end loop;
end procedure;

begin
Load_ROM(mem);
process(clk)
begin
	if rising_edge(clk) then
		address <= flags&instruction(7)&instruction(6)&instruction(5)&instruction(4)&count;
		enNewInstr <= '0';
		count <= count + 1;
		if count = "0100" then
			count <= "0000";
			enNewInstr <= '1';
		end if;
	end if;
end process;
process(address)
begin
	o <= mem(conv_integer(address));
end process;
end architecture CPUU;

--Random Access Memory
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
USE ieee.numeric_std.ALL;

entity RAM is
	port (address_data : in std_logic_vector(7 downto 0); 
			RAM_DATA_IN: in std_logic_vector(7 downto 0);
			RI, RO, MI: in std_logic;
			clk : in std_logic;
			RAM_DATA_OUT: out std_logic_vector(7 downto 0));
end RAM;

architecture RAMM of RAM is

signal RAM_ADDR : std_logic_vector(3 downto 0);

type RAM_ARRAY is array (0 to 15) of std_logic_vector (7 downto 0);

signal RAM: RAM_ARRAY  := ("00011111",
									"00101110",
									"11100000",
									"01001111",
									"00111101",
									"10001000",
									"01100000",
									"00000000",
									"11110000",
									"00000000",
									"00000000",
									"00000000",
									"00000000",
									"00010100",
									"00000010",
									"00000000"); 
begin
process(clk, MI, address_data, RAM_DATA_IN, RI)
begin
	if(MI = '1' and clk = '1') then
		RAM_ADDR <= address_data(3)&address_data(2)&address_data(1)&address_data(0);
	end if;
	if clk = '1' then
		if(RI = '1') then
			RAM(to_integer(unsigned(RAM_ADDR))) <= RAM_DATA_IN;
		end if;
	end if;
end process;
RAM_DATA_OUT <= RAM(to_integer(unsigned(RAM_ADDR))) when (RO = '1');
end architecture RAMM;

--Arithmetic Logic Unit (ALU)
library ieee;
use ieee.std_logic_1164.all;

entity FA is
	port (a, b, cin, su, en : in std_logic;
			c, cout : out std_logic);
end entity FA;

architecture arc of FA is
begin
process(a, b, en, su, cin)
begin
	c <=  en and ((a xor (su xor b))	xor cin);
	cout <=  (a and (su xor b)) or (cin and (a xor (su xor b)));
end process;
end architecture arc;

library ieee;
use ieee.std_logic_1164.all;

entity ALU is
	port (a, b : in std_logic_vector (7 downto 0);
			EO, SU : in std_logic;
			CY : out std_logic;
			s : out std_logic_vector (7 downto 0));
end entity;

architecture ALUU of ALU is

component FA
	port (a, b, cin, su, en : in std_logic;
			c, cout : out std_logic);
end component;

signal c0, c1, c2, c3, c4, c5, c6 : std_logic;

begin

u1: FA port map (a => a(0), b => b(0), su => SU, en => EO, cin => SU, cout => c0, c => s(0));
u2: FA port map (a => a(1), b => b(1), su => SU, en => EO, cin => c0, cout => c1, c => s(1));
u3: FA port map (a => a(2), b => b(2), su => SU, en => EO, cin => c1, cout => c2, c => s(2));
u4: FA port map (a => a(3), b => b(3), su => SU, en => EO, cin => c2, cout => c3, c => s(3));
u5: FA port map (a => a(4), b => b(4), su => SU, en => EO, cin => c3, cout => c4, c => s(4));
u6: FA port map (a => a(5), b => b(5), su => SU, en => EO, cin => c4, cout => c5, c => s(5));
u7: FA port map (a => a(6), b => b(6), su => SU, en => EO, cin => c5, cout => c6, c => s(6));
u8: FA port map (a => a(7), b => b(7), su => SU, en => EO, cin => c6, cout => CY, c => s(7));

end architecture ALUU;

--Component connection
library ieee;
use ieee.std_logic_1164.all;

entity com is
	port (clkraw : in std_logic;
			output : out std_logic_vector (7 downto 0));
end entity com;

architecture arc of com is

component ALU is
	port (a, b : in std_logic_vector (7 downto 0);
			EO, SU : in std_logic;
			CY : out std_logic;
			s : out std_logic_vector (7 downto 0));
end component;

component CPU is
	port (instruction : in std_logic_vector(7 downto 0);
			clk : in std_logic;
			flags : in std_logic_vector (1 downto 0);
			enNewInstr : out std_logic;
			o : out std_logic_vector (15 downto 0));
end component;

component reg is
	port (i : in std_logic_vector (7 downto 0);
			o, otoU : out std_logic_vector (7 downto 0);
			load, en, clk : in std_logic);
end component;

component inreg is
	port (i : in std_logic_vector(7 downto 0);
			II, IO, clk, enNew: in std_logic;
			po, co, instruction_current_data : out std_logic_vector(7 downto 0));
end component;

component cout is
	port (i : in std_logic_vector (7 downto 0);
			o : out std_logic_vector (7 downto 0);
			PCO, J, CE, clk : in std_logic);
end component;

component RAM is
	port (address_data : in std_logic_vector(7 downto 0); 
			RAM_DATA_IN: in std_logic_vector(7 downto 0);
			RI, RO, MI: in std_logic;
			clk : in std_logic;
			RAM_DATA_OUT: out std_logic_vector(7 downto 0));
end component;		

component clock is
	port (clk, HLT : in std_logic;
			o, orev : out std_logic);
end component;

component busreg is
	port (ram, inreg, procount, regA, regB, ALUB : in std_logic_vector (7 downto 0);
			RO, PCO, AO, BO, EO, IO : in std_logic;
			o : out std_logic_vector (7 downto 0));
end component;

component zerof is
	port (i : in std_logic_vector(7 downto 0);
			clk : in std_logic;
			o : out std_logic);
end component;

component flag is
	port (i : in std_logic_vector (1 downto 0);
			FI, clk : in std_logic;
			o : out std_logic_vector (1 downto 0));
end component;

signal command : std_logic_vector (15 downto 0);
signal InstructionRegisterToControlUnit : std_logic_vector (7 downto 0);
signal RAMtoBUS, InRegToBus, ProCountToBus, RegAtoBus, RegBtoBus, ALUtoBus, busout : std_logic_vector (7 downto 0);	
signal RegisterAtoALU, RegisterBtoALU, instruction_current_data : std_logic_vector (7 downto 0);
signal clk, clkrev : std_logic;
signal con, enInRegFromCPU : std_logic := '1';
signal carry, zero : std_logic;
signal FlagRegisterToCPUadd : std_logic_vector (1 downto 0);
signal conrev : std_logic := '0';

begin 

ProgramCounter : cout port map (i => busout, o => ProCountToBus, clk => clk, PCO => command(2), J => command(1), CE => command(3));
RegisterA : reg port map (i => busout, otoU => RegisterAtoALU, clk => clk, load => command(9), en => command(8), o => RegAtoBus);
RegisterB : reg port map (i => busout, otoU => RegisterBtoALU, clk => clk, load => command(5), en => conrev, o => RegBtoBus);
OutPutRegister : reg port map (i => busout, o => output, clk => clk, load => command(4), en => con);
InstructionRegister : inreg port map (i => busout, co => InstructionRegisterToControlUnit, po => InRegToBus, clk => clk, IO => command(11), II => command(10), enNew => enInRegFromCPU, instruction_current_data => instruction_current_data);
ArithmeticLogicUnit : ALU port map (a => RegisterAtoALU, b => RegisterBtoALU, s => ALUtoBUS, EO => command(7), SU => command(6), CY => carry);
RandomAccessMemory : RAM port map (address_data => busout, RAM_DATA_IN => busout, clk => clk, RAM_DATA_OUT => RAMtoBUS, RI => command(13), RO => command(12), MI => command(14));
ControlUnit : CPU port map (instruction => InstructionRegisterToControlUnit, clk => clkrev, o => command, enNewInstr => enInRegFromCPU, flags => FlagRegisterToCPUadd);
ClockControlUnit : clock port map (clk => clkraw, o => clk, orev => clkrev, hlt => command(15));
BUSregister : busreg port map (ram => RAMtoBUS, regA => RegAtoBus, regB => RegBtoBus, inreg => InRegToBus, procount => ProCountToBus, ALUB => ALUtoBus, o => busout, RO => command(12), PCO => command(2), AO => command(8), BO => conrev, EO => command(7), IO => command(11));
FlagRegister : flag port map (i(1) => carry, i(0)=> zero, FI => command(0), clk => clk, o => FlagRegisterToCPUadd);
ZeroFlagStatement : zerof port map (i => ALUtoBus, o => zero, clk => clk);

end architecture arc;