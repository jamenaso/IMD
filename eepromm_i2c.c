#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/i2c.h>
#include <linux/types.h>
#include <linux/uaccess.h>

/* String que define el nombre del dispositivo */
#define DEVICE_NAME  "memIMD"

static struct i2c_client *client_device;

static int16_t addr;

/*********************************************************************************
 * Definiciones de funciones del driver
 *********************************************************************************/
static int memIMD_open(struct inode *inode, struct file *file) {
	//int rd_ok = 0;
	char data[1];
	data[0] = 0;
	pr_info("La función open() a sido invocada.\n");
	//rd_ok = read(0x0000,data,1);
	/*
	if (rd_ok < 0) {
		pr_info("Error al abrir el driver memIMD.\n");
	} else {
		pr_info("Número bytes leídos en open(): %d\n", rd_ok);
		pr_info("Dato leído de dirección 0x0000: %x\n", data[0]);
		pr_info("Open del driver a sido exitosa...\n");
	}*/
	return 0;
}

static int memIMD_close(struct inode *inode, struct file *file) {
	pr_info("La función close() a sido invocada.\n");
	return 0;
}

static long memIMD_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
	pr_info("La función ioctl() a sido invocada.\n");
	if(cmd == 0x01){
		addr = arg & 0xFFFF;
		pr_info("Dirección de lectura establecida en: %d\n", addr);
		return 0;
	}else{
		return -1;
	}
}

static ssize_t memIMD_write(struct file *file, const char __user *buffer, size_t len, loff_t *offset) {
	int i;
	int amount = -1;
	char frame[len + 2];

	pr_info("write() fue invocada.\n");
	if(len >= 1){
		frame[0] = addr>>8;
		frame[1] = addr;
		for (i = 2; i < len + 2; i++){
			frame[i] = buffer[i];
		}
		amount = i2c_master_send(client_device, frame, len);
	}else{
		pr_info("Bytes insuficientes en la trama para escritura.\n");
	}
	if (amount < 0) {
		pr_info("Error escribiendo dato a memoria.\n");
	}
	return 0;
}

static ssize_t memIMD_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset) {

	char frame[2] = {addr>>8, addr};
	char data[len];
	int amount = -1;
	int ret;

	pr_info("read() fue invocada.\n");

	amount = i2c_master_send(client_device, frame, 2);
	if (amount < 0) {
		pr_info("Error escribiendo trama para lectura.\n");
		return amount;
	}

	amount = i2c_master_recv(client_device, data, len);
	if (amount != len) {
		pr_info("Error al leer los datos de la memoria.\n");
		return amount;
	}

	ret = copy_to_user(buffer, data, len);
	if(ret != 0){
		return -1;
	}
	return 0;
}

/* Declaración de una estructura del tipo file_operations */
static const struct file_operations my_dev_fops = {
		.owner = THIS_MODULE,
		.open = memIMD_open,
		.read = memIMD_read,
		.write = memIMD_write,
		.release = memIMD_close,
		.unlocked_ioctl = memIMD_ioctl,
};

/*--------------------------------------------------------------------------------*/

/* Declaración e inicialización de una estructura miscdevice */
static struct miscdevice memIMD_miscdevice = {
		.minor = MISC_DYNAMIC_MINOR,
		.name = DEVICE_NAME,
		.fops = &my_dev_fops, };

//************************************************************************************
/*
 * Definiciones de funciones probe y remove
 */

static int memIMD_probe(struct i2c_client *device_slave, const struct i2c_device_id *id) {
	int ret_val;

	pr_info("Ejecutando probe() memoria 24LC512\n");
	pr_info("Información del dispositivo detectado:\n");
	pr_info("\tNombre = %s\n", device_slave->name);
	pr_info("\tDirección = %#x\n", device_slave->addr);
	pr_info("\tDriver = %s\n", (device_slave->dev).driver->name);

	/* Registro del dispositivo con el kernel */
	ret_val = misc_register(&memIMD_miscdevice);

	if (ret_val != 0) {
		pr_err("No se registró el dispositivo %s\n", DEVICE_NAME);
		return ret_val;
	}

	pr_info("Dispositivo %s: \n", DEVICE_NAME);
	pr_info("Número minor asignado: %i\n", memIMD_miscdevice.minor);

	/*
	 * Se asigna el puntero del dispositivo detectado a la variable global
	 * del dispositivo cliente para trabajar con ese puntero en las funciones
	 * write y read.
	 */
	client_device = device_slave;

	return 0;
}

static int memIMD_remove(struct i2c_client *client) {

	/* Unregister del miscDevice del kernel */
	misc_deregister(&memIMD_miscdevice);

	pr_info("Módulo removido\n");

	return 0;
}

static const struct of_device_id memIMD_dt_ids[] = {
			{	.compatible = "mse,memIMD", },
			{ }
		};

MODULE_DEVICE_TABLE(of, memIMD_dt_ids);

static struct i2c_driver memIMD_driver = {
	.probe = memIMD_probe,
	.remove = memIMD_remove,
	.driver = {
			.name = "24LC256_driver",
			.owner = THIS_MODULE,
			.of_match_table = of_match_ptr(memIMD_dt_ids),
		},
};

/*----------------------------------------------------------------------*/

/**********************************************************************
 * Esta seccion define cuales funciones seran las ejecutadas al cargar o
 * remover el modulo respectivamente. Es hecho implicitamente,
 * termina declarando probe() y remove()
 **********************************************************************/
module_i2c_driver( memIMD_driver);

/**********************************************************************
 * Seccion sobre Informacion del modulo
 **********************************************************************/
MODULE_AUTHOR("Jairo A. Mena M <jamenaso@gmail.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Este módulo realiza comunicación I2C con memoria 24LC512...");
MODULE_INFO(mse_imd, "Trabajo práctico para la materia IMD");
