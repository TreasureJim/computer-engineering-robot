use btleplug::api::{Central, CentralEvent, Manager as _, ScanFilter};
use btleplug::api::{BDAddr, Peripheral};
use btleplug::platform::{Adapter, Manager, PeripheralId};
use futures::stream::StreamExt;
use futures::TryFutureExt;
use std::error::Error;
use std::process::exit;
use std::str::FromStr;

async fn get_central(manager: &Manager) -> Adapter {
    let adapters = manager.adapters().await.unwrap();
    adapters.into_iter().nth(0).unwrap()
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn Error>> {
    let bdaddr = std::env::args().nth(1).unwrap();
    let bdaddr = BDAddr::from_str(&bdaddr).unwrap();

    let manager = Manager::new().await?;

    let central = get_central(&manager).await;

    // start scanning for devices
    central.start_scan(ScanFilter::default()).await?;

    let dev_id = find_dev(&central, &bdaddr).await.unwrap_or_else(|| {
        eprintln!("Couldn't find device");
        exit(1);
    });

    println!("Found device!");

    let peripheral = central.peripheral(&dev_id).await.unwrap();
    let _ = peripheral.connect().unwrap_or_else(|_| {
        eprintln!("Couldn't connect to device");
        exit(1);
    });

    println!("Connected!");

    let _ = peripheral.discover_services().unwrap_or_else(|_| {
        eprintln!("E: discovering services");
        exit(1);
    });
    if peripheral.characteristics().is_empty() {
        println!("No characteristics");
    }

    for char in peripheral.characteristics() {
        println!("Char: {:?}", char);
    }

    Ok(())

    // let char = Characteristic {
    //     uuid: uuid_from_
    //     service_uuid: uuid_from_u16(0x2902),
    //     properties: CharPropFlags::default(),
    //     descriptors: BTreeSet::new()
    // };

    // loop {
    //     let response = peripheral.read(&char).await;
    //     println!("Received: {:?}", response);
    // }
}

async fn find_dev(adapter: &Adapter, bdaddr: &BDAddr) -> Option<PeripheralId> {
    let mut events = adapter.events().await;

    while let Some(event) = events.as_mut().unwrap().next().await {
        if let CentralEvent::DeviceDiscovered(id) = event {
            let peripheral = adapter.peripheral(&id).await.unwrap();
            if let Ok(Some(properties)) = peripheral.properties().await {
                if properties.address == *bdaddr {
                    return Some(id);
                }
            }
        }
    }

    None
}
