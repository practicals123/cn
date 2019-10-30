/*enableing pcap and mobility between wifi (point to point network) and 
csma (LAN) network.*/

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h" //new 
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"//new
#include "ns3/netanim-module.h"
#include "ns3/mobility-module.h"

// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1   n2   n3   n4
//    point-to-point  |    |    |    |
//                    ================
//                      LAN 10.1.2.0
//n0(IP:10.1.1.1) and n1(10.1.1.2) uses wifi (point to point application) network
//n1(10.1.2.1),n2(10.1.2.2),n3(10.1.2.3) and n4(10.1.2.4) used csma (LAN) network
//n1(10.1.1.2 and 10.1.2.1) is acting as gateway for both network having two ip address.

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SecondScriptExample");

int 
main (int argc, char *argv[])
{
  bool verbose = true;
  uint32_t nCsma = 3;

  CommandLine cmd;
  cmd.AddValue ("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);

  cmd.Parse (argc,argv);

  if (verbose)
    {
      LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

  nCsma = nCsma == 0 ? 1 : nCsma;

  NodeContainer p2pNodes;
  p2pNodes.Create (2);

  NodeContainer csmaNodes;
  csmaNodes.Add (p2pNodes.Get (1));
  csmaNodes.Create (nCsma);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer p2pDevices;
  p2pDevices = pointToPoint.Install (p2pNodes);

  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));

  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install (csmaNodes);

  InternetStackHelper stack;
  stack.Install (p2pNodes.Get (0));
  stack.Install (csmaNodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces = address.Assign (p2pDevices);

  address.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces;
  csmaInterfaces = address.Assign (csmaDevices);

  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (csmaNodes.Get (nCsma));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (csmaInterfaces.GetAddress (nCsma), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (p2pNodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

//Assigning fixed position to nodes. optional, but if skipped, gives warning for animation.
MobilityHelper mobility;
mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
mobility.Install (p2pNodes);
mobility.Install (csmaNodes);

//creating a bounding box for NetAnim
// AnimationInterface::SetBoundary(0,0,100,100);
//above line required ns3.19 and bellow version
  AnimationInterface anim("second.xml");
//client 0, x=10, y=20
// point to point network considered
  AnimationInterface::SetConstantPosition (p2pNodes.Get(0),10,20);
  AnimationInterface::SetConstantPosition (p2pNodes.Get(1),30,20);

//csma network considered node 1 is acting as a gateway for bothe nw

//AnimationInterface::SetConstantPosition (csmaNodes.Get(1),40,20);
//AnimationInterface::SetConstantPosition (csmaNodes.Get(2),50,20);
//AnimationInterface::SetConstantPosition (csmaNodes.Get(3),60,20);

//change the coordinator x and y values.
  AnimationInterface::SetConstantPosition (csmaNodes.Get(1),40,5);
  AnimationInterface::SetConstantPosition (csmaNodes.Get(2),50,10);
  AnimationInterface::SetConstantPosition (csmaNodes.Get(3),60,15);


  anim.EnablePacketMetadata(true);

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

//enable pcap all
  pointToPoint.EnablePcapAll ("second");

//eanble interface pcap of n2 is 1 node
  csma.EnablePcap ("second", csmaDevices.Get (1), true);

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
