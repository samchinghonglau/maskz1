package arch.sm213.machine.student;

import machine.AbstractMainMemory.InvalidAddressException;
import org.junit.Before;
import org.junit.Test;
import static org.junit.Assert.*;


public class MainMemoryTest{

  private MainMemory mainMemory;

  @Before
  public void createMemory(){
    mainMemory = new MainMemory(128);
  }

  // To test isAccessAligned() method, I used two tests to make sure it can produce both true and false boolean by giving
  // addresses that is the nth power of the length and not the nth power of the length.
  @Test
  public void testIsAccessAlignedTrue(){
    assertTrue(mainMemory.isAccessAligned(4,2));
    assertTrue(mainMemory.isAccessAligned(8,4));
    assertTrue(mainMemory.isAccessAligned(0,8));
    assertTrue(mainMemory.isAccessAligned(9,3));
  }

  @Test
  public void testIsAccessAlignedFalse(){
    assertFalse(mainMemory.isAccessAligned(5,2));
    assertFalse(mainMemory.isAccessAligned(3,4));
    assertFalse(mainMemory.isAccessAligned(7,3));
  }

  // For testing the bytesToInteger(), it should be pretty straightforward using several bytes, call the function and
  // use assertEquals() to see if they match with the expected result.
  @Test
  public void testBytesToInteger(){
    int n1 = mainMemory.bytesToInteger((byte) 0x10, (byte) 0x20, (byte) 0x30, (byte) 0x40);
    assertEquals(n1, (int) 0x10203040);
    int n2 = mainMemory.bytesToInteger((byte) 0, (byte) 0, (byte) 0, (byte) 0);
    assertEquals(n2, (int) 0x0);
    int n3 = mainMemory.bytesToInteger((byte) 0x15, (byte) 0x37, (byte) 0xff, (byte) 0x80);
    assertEquals(n3, (int) 0x1537ff80);
  }

  // For testing the integerToBytes(), it should also be pretty straightforward using an int, call the function and store it into
  // an empty array and use assertEquals() to see if they match.
  @Test
  public void testIntegerToBytes(){
    int a = 0xff1926b5;
    byte bytes[];
    bytes = mainMemory.integerToBytes(a);
    assertEquals((byte) 0xff, bytes[0]);
    assertEquals((byte) 0x19, bytes[1]);
    assertEquals((byte) 0x26, bytes[2]);
    assertEquals((byte) 0xb5, bytes[3]);
  }

  // To test get set, at first I used two separate methods for testing: testGet() and testSet(), but later I figured that
  // it might be more efficient to use only one method that is set the address first and get the value from the set address.
  // So I created two arrays and set their address starting from 5 and 9 then get those arrays back at address 5-8 9-12,
  // then use assertEquals() to see if they are correct.
  @Test
  public void testGetSet() throws InvalidAddressException {
    byte[] a0 = {5, 18, 22, 33};
    byte[] b0 = {10, 20, 30, 40};
    mainMemory.set(5, a0);
    mainMemory.set(9, b0);
    byte[] a1 = mainMemory.get(5, 4);
    byte[] b1 = mainMemory.get(9, 4);
    assertEquals(5, (int) a1[0]);
    assertEquals(18, (int) a1[1]);
    assertEquals(22, (int) a1[2]);
    assertEquals(33, (int) a1[3]);
    assertEquals(10, (int) b1[0]);
    assertEquals(20, (int) b1[1]);
    assertEquals(30, (int) b1[2]);
    assertEquals(40, (int) b1[3]);
  }

}
