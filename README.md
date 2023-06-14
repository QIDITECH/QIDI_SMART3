<p align="center"><img src="https://github.com/Rainboooom/test/blob/main/QIDI.png" height="240" alt="QIDI's logo" /></p>
<p align="center"><a href="/LICENSE"><img alt="GPL-V3.0 License" src="https://github.com/Rainboooom/test/blob/main/qidi.svg"></a></p>

# Document Instructions
QIDISmart3 is a server-side software that interacts with the screen in the smart3 model system. This document is prepared for users who want to manually update the server, and we will provide the specific update process in this description.  
*Please note that manual updates may affect normal after-sales service, so it is best to automatically update through the machine with packaged files.*

## Detailed update process
1. Connect machines to the network and connect through SSH
2. Log in as root.The password is `makerbase`
3. After logging in, enter the following code block
```shell
cd /root
rm -rf xindi
git clone https://github.com/Rainboooom/QIDISmart3.git
```
4. Code cloning may take some time, please be patient. If there are certificate issues, please confirm if the system time is correct. After the code cloning is completed, enter the following code block
```shell
mv QIDI_SMART3 xindi
cd /root/xindi/build
cmake ..
make
```
5. The complete code compilation also takes some time, patiently wait for the compilation to complete, shut down and wait for 20 seconds before starting.

## Others

The 3D printers of QIDI are based on Klipper.On the basic of Klipper open source project, we have made some modifications to it's source code to meet some of the user's needs.At the same time, we have also made modifications to Moonraker and Fluid, so that the screens we set can correspond to the operations on the page.
Thanks to the developers and maintainers of these open source projects.Please consider using or supporting these powerful projects.

 Software | original edition | QIDI edition
 ----|----|----
**Klipper** | **[link]()** | **[link]()**
**Fluidd** | **[link]()** | **[link]()**
**Moonraker** | **[link]()** | **[link]()**









  
