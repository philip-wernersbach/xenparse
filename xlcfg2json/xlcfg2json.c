/*
 * xlcfg2json.c
 * 
 * Part of xlcfg2json
 * 
 * Copyright (c) 2013 Philip Wernersbach & Jacobs Automation
 * All rights reserved.
 * 
 * This code is licensed under the 2-Clause BSD License.
 * The license text is in the LICENSE file.
 */

/* System includes */
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>

/* Library includes */
#include <libxlu_internal.h>
#include <cJSON/cJSON.h>

#define REPORT_FILE_SUFFIX ".report"

void read_config_into_json(XLU_ConfigSetting *settings, cJSON *json_root) {
	cJSON *obj = cJSON_CreateArray();
	
	unsigned int i;
	for (i = 0; i < settings->nvalues; i++) {
		long int_value = (long)NULL;
		char *end;
		
		int_value = strtol(settings->values[i], &end, 10);
		
		if (*end)
			cJSON_AddItemToArray(obj, cJSON_CreateString(settings->values[i]));
		else
			cJSON_AddItemToArray(obj, cJSON_CreateNumber(int_value));
	}
	
	if (settings->avalues != 1)
		cJSON_AddItemToObject(json_root, settings->name, obj);
	else
		cJSON_AddItemToObject(json_root, settings->name, cJSON_GetArrayItem(obj, 0));
	
	if (settings->next != NULL)
		read_config_into_json(settings->next, json_root);
}

int main(int argc, char **argv) {
	XLU_Config *vm_config = NULL;
	char *vm_config_file = NULL;
	char *vm_config_report_file = NULL;
	
	FILE *json_output_file = NULL;
	char *json_output_file_name = NULL;
	
	unsigned int retval = 0;
	
	if (argc == 3) {
		FILE *tmp = NULL;
		
		vm_config_file = malloc(strlen(argv[1]) + 1);
		vm_config_report_file = malloc(strlen(argv[1]) + strlen(REPORT_FILE_SUFFIX) + 1);
		
		json_output_file_name = malloc(strlen(argv[2]) + 1);
		
		strcpy(vm_config_file, argv[1]);
		strcpy(vm_config_report_file, vm_config_file);
		strcpy(json_output_file_name, argv[2]);
		strcat(vm_config_report_file, REPORT_FILE_SUFFIX);
		
		tmp = fopen(vm_config_file, "r");
		if (tmp == NULL) {
			fprintf(stderr, "ERROR: Cannot open xl config file.\n");
			retval = 1;
		} else {
			fclose(tmp);
			
			tmp = fopen(vm_config_file, "rw");
			if (tmp == NULL) {
				fprintf(stderr, "ERROR: Cannot open xl config report file.\n");
				retval = 1;
			} else {			
				fclose(tmp);
				
				json_output_file = fopen(json_output_file_name, "w");
				if (json_output_file == NULL) {
					fprintf(stderr, "ERROR: Cannot open JSON output file.\n");
					retval = 1;
				} else {
					vm_config = xlu_cfg_init(NULL, vm_config_report_file);
					xlu_cfg_readfile(vm_config, vm_config_file);
				}
			}
		}
	} else {
		fprintf(stderr, "ERROR: Must specify xl config file and JSON output file on command line.\n");
		fprintf(stderr, "USAGE: %s <xl config file> <JSON output file>\n", argv[0]);
		
		retval = 1;
	}
	
	if ((vm_config != NULL) && (vm_config->settings != NULL)) {
		cJSON *json_root = cJSON_CreateObject();
		
		read_config_into_json(vm_config->settings, json_root);
		fputs(cJSON_Print(json_root), json_output_file);
		
		cJSON_Delete(json_root);
	}
	
	if (json_output_file != NULL)
		fclose(json_output_file);
	
	if (vm_config != NULL)
		xlu_cfg_destroy(vm_config);
	
	if (vm_config_report_file != NULL)
		free(vm_config_report_file);
	
	if (vm_config_file != NULL)
		free(vm_config_file);
	
	return retval;
}
